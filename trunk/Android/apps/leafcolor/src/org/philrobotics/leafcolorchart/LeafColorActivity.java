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
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.telephony.SmsManager;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.Toast;

public class LeafColorActivity extends Activity implements CvCameraViewListener, SensorEventListener {
    private static final String  TAG                 = "AndLCC";

    public static final int      VIEW_MODE_LEAF      = 0;
    public static final int      VIEW_MODE_ZOOM      = 1;
    public static final int      VIEW_MODE_LEVELING     = 2;

    private MenuItem             mItemPreviewLeaf;
    private MenuItem             mItemPreviewZoom;
    private MenuItem             mItemPreviewLevel;
    private MenuItem             mItemCalibrate;
    private MenuItem             mItemSendData;
    private CameraBridgeViewBase mOpenCvCameraView;

    private Size                 mSizeRgba;
    private Size                 mSizeLeafBox;

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
    
    private ImageView            mColorchartView;
    
    // leaf/green hsv table
    private ColorTable           mColTable;
    private Scalar               mHsvCal2;
    private Scalar               mHsvCal5;
    
    private Scalar               mRGBmean;
    private Scalar               mHSVmean;
    private int                  mLeafLevel = -1;
    private boolean              mbPause = false;
    
    // land leveling
    private SensorManager        mSensorManager;
    private int                  mOrientationSensor;
    private float                mRollAngle;
    private float                mPitchAngle;

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
    @SuppressWarnings("deprecation")
    @Override
    public void onCreate(Bundle savedInstanceState) {
        Log.i(TAG, "called onCreate");
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        setContentView(R.layout.main);

        mOpenCvCameraView = (CameraBridgeViewBase) findViewById(R.id.leaf_color_activity_surface_view);
        mOpenCvCameraView.setCvCameraViewListener(this);
        
        mColorchartView = (ImageView)findViewById(R.id.colorchartview);
        mColorchartView.setImageResource(R.raw.six_panel_lcc);
        
        mSensorManager = (SensorManager)getSystemService(SENSOR_SERVICE);
        mOrientationSensor = Sensor.TYPE_ORIENTATION;
        mSensorManager.registerListener(sensorEventListener,
        		mSensorManager.getDefaultSensor(mOrientationSensor), mSensorManager.SENSOR_DELAY_GAME);
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
        
        mSensorManager.unregisterListener(sensorEventListener);
    }
    
    public void onBackPressed() {
    	new AlertDialog.Builder(this)
    	.setTitle("Green Mind")
    	.setMessage("Close this Application?")
    	.setPositiveButton("Yes", new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface dialog, int which) {
				finish();				
			}
		})
		.setNegativeButton("No", null)
		.show();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        Log.i(TAG, "called onCreateOptionsMenu");
        //mItemPreviewLeaf  = menu.add("Leaf Color");
        mItemPreviewLeaf  = menu.add("Pause");
        mItemPreviewZoom  = menu.add("Zoom");
        mItemPreviewLevel = menu.add("Land Level");
        mItemCalibrate  = menu.add("Calibrate");
        mItemSendData  = menu.add("Send Data");
        return true;
    }
    
    private void calibrate(int level){
    	if(level==2){
    		new AlertDialog.Builder(this)
        	.setTitle("Calibrate level 2")
        	.setMessage("now on position 2?")
        	.setPositiveButton("Yes", new DialogInterface.OnClickListener() {
    			public void onClick(DialogInterface dialog, int which) {
    				mHsvCal2 = mHSVmean;
    			}
    		})
    		.setNegativeButton("Cancel", null)
    		.show();
    	}
    	else if(level==5){
    		new AlertDialog.Builder(this)
        	.setTitle("Calibrate level 5")
        	.setMessage("now on position 5?")
        	.setPositiveButton("Yes", new DialogInterface.OnClickListener() {
    			public void onClick(DialogInterface dialog, int which) {
    				mHsvCal5 = mHSVmean;
    				mColTable.calibrate(mHsvCal2, mHsvCal5);
    			}
    		})
    		.setNegativeButton("Cancel", null)
    		.show();    		
    	}
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        Log.i(TAG, "called onOptionsItemSelected; selected item: " + item);
        if (item == mItemPreviewLeaf){
            viewMode = VIEW_MODE_LEAF;
            if(mbPause){
            	mbPause = false;
            	mItemPreviewLeaf.setTitle("Pause");
            }
            else{
            	mbPause = true;
            	mItemPreviewLeaf.setTitle("Run Leaf Color");
            }
        }
        else if (item == mItemPreviewZoom){
            viewMode = VIEW_MODE_ZOOM;
        }
        else if (item == mItemPreviewLevel){
            viewMode = VIEW_MODE_LEVELING;
        }
        else if (item == mItemCalibrate){
        	calibrate(5);
        	calibrate(2);
        	//calibrate(5); //?!?
        }
        else if (item == mItemSendData){
        	// pause view
        	mbPause = true;
        	mItemPreviewLeaf.setTitle("Run Leaf Color");
        	
        	//String phoneNo = "09217529353";
        	//String sms = "hello garci";
        	
            String phoneNo = "09396957027";
        	//String sms = String.format("[Green Mind] Leaf grade = %.1f", ((double)mLeafLevel)/10.0);
            String sms = String.format("%.1f", ((double)mLeafLevel)/10.0);
        	
        	try {
				SmsManager smsManager = SmsManager.getDefault();
				smsManager.sendTextMessage(phoneNo, null, sms, null, null);
				Toast.makeText(getApplicationContext(), "SMS Sent!",
							Toast.LENGTH_LONG).show();
			  } catch (Exception e) {
				Toast.makeText(getApplicationContext(),
					"SMS faild, please try again later!",
					Toast.LENGTH_LONG).show();
				e.printStackTrace();
			  }
        }

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
        
        mColTable = new ColorTable();
        mRGBmean = new Scalar(0,0,0);
        mHSVmean = new Scalar(0,0,0);
    }
    
    private int GreenMatch(int bH, int bS, int bV){
    	if( bV < 20 ) return -1; // black
		
		Core.putText(mRgba, String.format("HSV(%d,%d,%d)", bH, bS, bV),
        		new Point(5, mSizeRgba.height - 5),
    			Core.FONT_HERSHEY_PLAIN, 1, new Scalar(255, 255, 128, 255), 1);
		
		if( bH<30 || bH>115  ) return -1; // far from green hue
		
		return mColTable.lookup(bH, bS, bV);
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
        
        mSizeLeafBox = mLeafWindow.size();
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

        	Core.rectangle( mLeafWindow, new Point(0, 0), new Point(mSizeLeafBox.width - 1, mSizeLeafBox.height - 1), new Scalar(255, 0, 0, 255), 2 );
            
        	if(!mbPause){ // recalculate
        		// RGB average values
                mRGBmean = Core.mean(mLeafWindow);
                
                // HSV color space
                Imgproc.cvtColor(mLeafWindow, mIntermediateMat, Imgproc.COLOR_RGB2HSV_FULL);
                
                Imgproc.calcHist(Arrays.asList(mIntermediateMat), mChannels[0], mMat0, mHist, mHistSize, mRanges);
                Core.normalize(mHist, mHist, mSizeRgba.height/2, 0, Core.NORM_INF);
                mHist.get(0, 0, mBuff);
                
                // HSV average values
                mHSVmean = Core.mean(mIntermediateMat);
                
                // color matching
                mLeafLevel = GreenMatch((int)mHSVmean.val[0], (int)mHSVmean.val[1], (int)mHSVmean.val[2]);
        	}
        	
            Core.putText(mRgba, String.format("RGB(%.0f,%.0f,%.0f)", mRGBmean.val[0], mRGBmean.val[1], mRGBmean.val[2]),
            		new Point(5, mSizeRgba.height - 20),
        			Core.FONT_HERSHEY_PLAIN, 1, new Scalar(255, 255, 128, 255), 1);
            
            // plot Hue histogram
            int thickness = 5;
            for(int h=1; h<mHistSizeNum; h++) {
            	mP1.x = mP2.x = 10 + thickness*h; // offset
                mP1.y = mSizeRgba.height - 35;
                mP2.y = mP1.y - (int)mBuff[h] / 2;
                Core.line(mRgba, mP1, mP2, mColorsHue[h], thickness-1);
            }
            
            // show arrow pointer
            if( mLeafLevel>=0 ){
            	double offset = mLeafLevel * ( (mSizeRgba.height-20) / mColTable.getLength()); // fix me
            	Core.putText(mRgba, String.format("%.1f->", (double)mLeafLevel/10.0 + 0.5), // start with "0.5"
            			new Point(mSizeRgba.width-100, mSizeRgba.height - offset),
            			Core.FONT_HERSHEY_SCRIPT_SIMPLEX, 1, new Scalar(128, 255, 255, 255), 2);
            }
            
            break;
            
        case LeafColorActivity.VIEW_MODE_ZOOM:
           if ((mZoomCorner == null) || (mZoomWindow == null) || (mRgba.cols() != mSizeRgba.width) || (mRgba.height() != mSizeRgba.height))
               CreateAuxiliaryMats();
           Imgproc.resize(mZoomWindow, mZoomCorner, mZoomCorner.size());

           Size wsize = mZoomWindow.size();
           Core.rectangle(mZoomWindow, new Point(1, 1), new Point(wsize.width - 2, wsize.height - 2), new Scalar(255, 0, 0, 255), 2);
           break;
           
        case LeafColorActivity.VIEW_MODE_LEVELING:
        	if ( (mSizeRgba == null) || (mRgba.cols() != mSizeRgba.width) || (mRgba.height() != mSizeRgba.height) )
                CreateAuxiliaryMats();
        	
        	//mColorchartView.setVisibility(ImageView.INVISIBLE);
        	
        	Core.putText(mRgba,String.format("Roll (%.0f) Pitch (%.0f)", mRollAngle, mPitchAngle), new Point(5, mSizeRgba.height - 5),
          			Core.FONT_HERSHEY_PLAIN, 1, new Scalar(255, 0, 0, 255), 1) ;
        	
        	double w = mSizeRgba.width/2;
        	double h = mSizeRgba.height/2;
        	double angle;
        	
        	Point p1 = new Point();
        	angle = (double)(360-mRollAngle) * Math.PI/180;
        	p1.x = Math.cos(angle) * w / 2;
        	p1.y = Math.sin(angle) * h / 2;
        	Core.line(mRgba, new Point(w+p1.x, h+p1.y), new Point(w-p1.x, h-p1.y), new Scalar(255,0,255,255), 2);
        	
        	angle = (double)(360+90-mRollAngle) * Math.PI/180;
        	p1.x = Math.cos(angle) * w / 2;
        	p1.y = Math.sin(angle) * h / 2;
        	Core.line(mRgba, new Point(w+p1.x, h+p1.y), new Point(w-p1.x, h-p1.y), new Scalar(255,0,255,255), 2);
        	
        	break;
        
	    }
        return mRgba;
    }
    
    final SensorEventListener sensorEventListener = new SensorEventListener() {
		
		@SuppressWarnings("deprecation")
		public void onSensorChanged(SensorEvent event) {
			if(event.sensor.getType() == Sensor.TYPE_ORIENTATION){
				mRollAngle = event.values[0];
				mPitchAngle = event.values[1];
				
				Log.i(TAG, "Roll: " + String.valueOf(mRollAngle));
			}
		}
		
		public void onAccuracyChanged(Sensor sensor, int accuracy) {
			// TODO Auto-generated method stub
			
		}
	};

	public void onAccuracyChanged(Sensor sensor, int accuracy) {
		// TODO Auto-generated method stub
		
	}

	public void onSensorChanged(SensorEvent event) {
		// TODO Auto-generated method stub
		
	}
}
