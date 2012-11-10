package org.philrobotics.leafcolorchart;

import java.util.Arrays;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.MatOfFloat;
import org.opencv.core.MatOfInt;
import org.opencv.core.Point;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener;
import org.opencv.imgproc.Imgproc;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageView;

public class LeafColorActivity extends Activity implements CvCameraViewListener {
    private static final String  TAG                 = "AndLCC";

    public static final int      VIEW_MODE_LEAF      = 0;
    public static final int      VIEW_MODE_CANNY     = 1;
    public static final int      VIEW_MODE_ZOOM      = 2;

    private MenuItem             mItemPreviewCanny;
    private MenuItem             mItemPreviewZoom;
    private MenuItem             mItemPreviewLeaf;
    private CameraBridgeViewBase mOpenCvCameraView;

    private Size                 mSizeRgba;

    private Mat                  mRgba;
    private Mat                  mIntermediateMat;
    private Mat                  mHist;
    private Mat                  mMat0;
    private MatOfInt             mChannels[];
    private MatOfInt             mHistSize;
    private int                  mHistSizeNum;
    private MatOfFloat           mRanges;
    private Scalar               mColorsHue[];
    private Point                mP1;
    private Point                mP2;
    private float                mBuff[];
    private Mat                  mRgbaInnerWindow;
    private Mat                  mGrayInnerWindow;
    private Mat                  mZoomWindow;
    private Mat                  mZoomCorner;
    private Mat                  mLeafWindow;
    
    // leaf/green hsv table
    private int[]                mGreenHues;
    private int[]                mGreenSats;
    private int[]                mGreenVals;

    public static int           viewMode = VIEW_MODE_LEAF;

    private BaseLoaderCallback  mLoaderCallback = new BaseLoaderCallback(this) {
        @Override
        public void onManagerConnected(int status) {
            switch (status) {
                case LoaderCallbackInterface.SUCCESS:
                {
                    Log.i(TAG, "OpenCV loaded successfully");
                    mOpenCvCameraView.enableView();
                } break;
                default:
                {
                    super.onManagerConnected(status);
                } break;
            }
        }
    };

    public LeafColorActivity() {
        Log.i(TAG, "Instantiated new " + this.getClass());
    }

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        Log.i(TAG, "called onCreate");
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        setContentView(R.layout.main);

        mOpenCvCameraView = (CameraBridgeViewBase) findViewById(R.id.leaf_color_activity_surface_view);
        mOpenCvCameraView.setCvCameraViewListener(this);
        
        ImageView colorchartView = (ImageView)findViewById(R.id.colorchartview);
        colorchartView.setImageResource(R.raw.six_panel_lcc);
    }

    @Override
    public void onPause()
    {
        if (mOpenCvCameraView != null)
            mOpenCvCameraView.disableView();
        super.onPause();
    }

    @Override
    public void onResume()
    {
        super.onResume();
        OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_3, this, mLoaderCallback);
    }

    public void onDestroy() {
        super.onDestroy();
        if (mOpenCvCameraView != null)
            mOpenCvCameraView.disableView();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        Log.i(TAG, "called onCreateOptionsMenu");
        mItemPreviewLeaf  = menu.add("Leaf Color");
        mItemPreviewCanny = menu.add("Canny");
        mItemPreviewZoom  = menu.add("Zoom");        
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        Log.i(TAG, "called onOptionsItemSelected; selected item: " + item);
        if (item == mItemPreviewLeaf)
            viewMode = VIEW_MODE_LEAF;
        else if (item == mItemPreviewCanny)
            viewMode = VIEW_MODE_CANNY;
        else if (item == mItemPreviewZoom)
            viewMode = VIEW_MODE_ZOOM;

        return true;
    }

    public void onCameraViewStarted(int width, int height) {
        mRgba = new Mat();
        mIntermediateMat = new Mat();
        mHist = new Mat();
        mChannels = new MatOfInt[] { new MatOfInt(0), new MatOfInt(1), new MatOfInt(2) };
        mHistSizeNum = 25;
        mBuff = new float[mHistSizeNum];
        mHistSize = new MatOfInt(mHistSizeNum);
        mRanges = new MatOfFloat(0f, 256f);
        mMat0  = new Mat();
        mColorsHue = new Scalar[] {
                new Scalar(255, 0, 0, 255),   new Scalar(255, 60, 0, 255),  new Scalar(255, 120, 0, 255), new Scalar(255, 180, 0, 255), new Scalar(255, 240, 0, 255),
                new Scalar(215, 213, 0, 255), new Scalar(150, 255, 0, 255), new Scalar(85, 255, 0, 255),  new Scalar(20, 255, 0, 255),  new Scalar(0, 255, 30, 255),
                new Scalar(0, 255, 85, 255),  new Scalar(0, 255, 150, 255), new Scalar(0, 255, 215, 255), new Scalar(0, 234, 255, 255), new Scalar(0, 170, 255, 255),
                new Scalar(0, 120, 255, 255), new Scalar(0, 60, 255, 255),  new Scalar(0, 0, 255, 255),   new Scalar(64, 0, 255, 255),  new Scalar(120, 0, 255, 255),
                new Scalar(180, 0, 255, 255), new Scalar(255, 0, 255, 255), new Scalar(255, 0, 215, 255), new Scalar(255, 0, 85, 255),  new Scalar(255, 0, 0, 255)
        };
        mP1 = new Point();
        mP2 = new Point();
        
        mGreenHues = new int[] {
				71, 71, 71, 72, 72, 72, 73, 73, 74, 73,
				74, 75, 75, 75, 76, 77, 77, 78, 78, 79,
				80, 80, 81, 81, 82, 83, 83, 85, 86, 87,
				87, 89, 90, 90, 92, 93, 95, 96, 97, 100,
				100, 102, 104, 107, 108, 110, 114, 115, 118, 123,
				126, 127, 132, 134, 136, 140, 142, 145, 148, 150 };
        mGreenSats = new int[] {
				211, 211, 209, 208, 208, 205, 205, 204, 202, 201,
				201, 198, 197, 197, 195, 194, 193, 190, 189, 189,
				188, 185, 184, 184, 180, 179, 178, 175, 174, 173,
				169, 169, 167, 163, 162, 160, 157, 156, 154, 150,
				147, 146, 145, 139, 138, 137, 131, 129, 128, 128,
				132, 134, 138, 143, 144, 149, 155, 160, 162, 169 };
        mGreenVals = new int[] {
				145, 143, 142, 141, 139, 138, 137, 135, 134, 132,
				131, 130, 128, 127, 126, 124, 123, 122, 120, 119,
				118, 116, 115, 114, 112, 111, 109, 108, 107, 105,
				104, 103, 101, 100, 99, 97, 96, 95, 93, 92, 
				90, 89, 88, 86, 85, 84, 82, 81, 80, 78, 
				77, 76, 74, 73, 71, 70, 69, 67, 66, 65 };
    }
    
    private int GreenMatch(int bH, int bS, int bV){
    	int ret = -1;
    	
    	if( bV < 20 ) return -1; // black
		
		Core.putText(mRgba, String.format("HSV(%d,%d,%d)", bH, bS, bV),
        		new Point(5, mSizeRgba.height - 5),
    			Core.FONT_HERSHEY_PLAIN, 1, new Scalar(255, 255, 128, 255), 1);
		
		if( bH<50 || bH>170  ) return -1; // far from green hue
		
		// Color matching
		double near = 128;		
		for(int i=0; i<mGreenHues.length; i++){    		
    		double distance = (bH-mGreenHues[i]) * (bH-mGreenHues[i]);
    		distance += ( (bS-mGreenSats[i]) * (bS-mGreenSats[i]) );
    		distance += ( (bV-mGreenVals[i]) * (bV-mGreenVals[i]) );
    		distance = Math.sqrt(distance);
    		
    		if( distance < near ){
    			near = distance;
    			ret = i;
    			Log.i(TAG, String.format("hsv(%d,%d,%d) err = %.1f, level = %d",
    					bH, bS, bV, distance, i));
    		}
    	}
    	
    	return ret;
    }

    private void CreateAuxiliaryMats() {
        if (mRgba.empty())
            return;

        mSizeRgba = mRgba.size();

        int rows = (int) mSizeRgba.height;
        int cols = (int) mSizeRgba.width;

        int left = cols / 8;
        int top = rows / 8;

        int width = cols * 3 / 4;
        int height = rows * 3 / 4;

        if (mRgbaInnerWindow == null)
            mRgbaInnerWindow = mRgba.submat(top, top + height, left, left + width);
        
        if (mZoomCorner == null)
            mZoomCorner = mRgba.submat(0, rows / 2 - rows / 10, 0, cols / 2 - cols / 10);

        if (mZoomWindow == null)
            mZoomWindow = mRgba.submat(rows / 2 - 9 * rows / 100, rows / 2 + 9 * rows / 100, cols / 2 - 9 * cols / 100, cols / 2 + 9 * cols / 100);
        
        if (mLeafWindow == null)
        	mLeafWindow = mRgba.submat( rows / 10, 9 * rows / 10, cols / 2 - cols / 10, cols / 2 + cols / 10);
    }

    public void onCameraViewStopped() {
        // Explicitly deallocate Mats
        if (mZoomWindow != null)
            mZoomWindow.release();
        if (mZoomCorner != null)
            mZoomCorner.release();
        if (mGrayInnerWindow != null)
            mGrayInnerWindow.release();
        if (mRgbaInnerWindow != null)
            mRgbaInnerWindow.release();
        if (mRgba != null)
            mRgba.release();
        if (mIntermediateMat != null)
            mIntermediateMat.release();

        if (mLeafWindow != null)
        	mLeafWindow.release();
        
        mRgba = null;
        mIntermediateMat = null;
        mRgbaInnerWindow = null;
        mGrayInnerWindow = null;
        mZoomCorner = null;
        mZoomWindow = null;
        mLeafWindow = null;
    }

    public Mat onCameraFrame(Mat inputFrame) {
        inputFrame.copyTo(mRgba);

        switch (LeafColorActivity.viewMode) {
        
        case LeafColorActivity.VIEW_MODE_LEAF:
        	if ( (mLeafWindow == null) || (mSizeRgba == null) || (mRgba.cols() != mSizeRgba.width) || (mRgba.height() != mSizeRgba.height) )
                CreateAuxiliaryMats();

        	Size lsize = mLeafWindow.size();
            Core.rectangle( mLeafWindow, new Point(0, 0), new Point(lsize.width - 1, lsize.height - 1), new Scalar(255, 0, 0, 255), 2 );
            
            // RGB average values
            Scalar rgb = Core.mean(mLeafWindow);            
            Core.putText(mRgba, String.format("RGB(%.0f,%.0f,%.0f)", rgb.val[0], rgb.val[1], rgb.val[2]),
            		new Point(5, mSizeRgba.height - 20),
        			Core.FONT_HERSHEY_PLAIN, 1, new Scalar(255, 255, 128, 255), 1);
            
            // HSV color space
            Imgproc.cvtColor(mLeafWindow, mIntermediateMat, Imgproc.COLOR_RGB2HSV_FULL);
            
            // HSV average values
            Scalar hsv = Core.mean(mIntermediateMat);
            
            // color matching
            int level = GreenMatch((int)hsv.val[0], (int)hsv.val[1], (int)hsv.val[2]);
            
            // show arrow pointer
            if( level>=0 ){
            	double offset = level * ( (mSizeRgba.height-20) / mGreenHues.length); // fix me
            	level += 5; // start with "0.5"
            	Core.putText(mRgba, String.format("%.1f->", (double)level/10.0),
            			new Point(mSizeRgba.width-100, mSizeRgba.height - offset),
            			Core.FONT_HERSHEY_SCRIPT_SIMPLEX, 1, new Scalar(128, 255, 255, 255), 2);
            }
            
            // plot Hue histogram
            Imgproc.calcHist(Arrays.asList(mIntermediateMat), mChannels[0], mMat0, mHist, mHistSize, mRanges);
            Core.normalize(mHist, mHist, mSizeRgba.height/2, 0, Core.NORM_INF);
            mHist.get(0, 0, mBuff);
            
            int thickness = 5;
            for(int h=1; h<mHistSizeNum; h++) {
            	mP1.x = mP2.x = 10 + thickness*h; // offset
                mP1.y = mSizeRgba.height - 35;
                mP2.y = mP1.y - (int)mBuff[h] / 2;
                Core.line(mRgba, mP1, mP2, mColorsHue[h], thickness-1);
            }
            
            break;
        	
        case LeafColorActivity.VIEW_MODE_CANNY:
            if ((mRgbaInnerWindow == null) || (mGrayInnerWindow == null) || (mRgba.cols() != mSizeRgba.width) || (mRgba.height() != mSizeRgba.height))
               CreateAuxiliaryMats();
           Imgproc.Canny(mRgbaInnerWindow, mIntermediateMat, 80, 90);
           Imgproc.cvtColor(mIntermediateMat, mRgbaInnerWindow, Imgproc.COLOR_GRAY2BGRA, 4);
           break;

       case LeafColorActivity.VIEW_MODE_ZOOM:
           if ((mZoomCorner == null) || (mZoomWindow == null) || (mRgba.cols() != mSizeRgba.width) || (mRgba.height() != mSizeRgba.height))
               CreateAuxiliaryMats();
           Imgproc.resize(mZoomWindow, mZoomCorner, mZoomCorner.size());

           Size wsize = mZoomWindow.size();
           Core.rectangle(mZoomWindow, new Point(1, 1), new Point(wsize.width - 2, wsize.height - 2), new Scalar(255, 0, 0, 255), 2);
           break;
        }

        return mRgba;
    }
}
