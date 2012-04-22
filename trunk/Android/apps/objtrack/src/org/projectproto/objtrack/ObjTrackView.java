package org.projectproto.objtrack;

import android.content.Context;
import android.graphics.Bitmap;

class ObjTrackView extends SampleViewBase {

    public ObjTrackView(Context context) {
        super(context);
    }

    @Override
    protected Bitmap processFrame(byte[] data) {
        int frameSize = getFrameWidth() * getFrameHeight();
        int[] rgba = new int[frameSize];

        CircleObjectTrack(getFrameWidth(), getFrameHeight(), data, rgba, ObjTrackActivity.bShowTresholded);

        Bitmap bmp = Bitmap.createBitmap(getFrameWidth(), getFrameHeight(), Bitmap.Config.ARGB_8888);
        bmp.setPixels(rgba, 0/* offset */, getFrameWidth() /* stride */, 0, 0, getFrameWidth(), getFrameHeight());
        
        return bmp;
    }

    public native void CircleObjectTrack(int width, int height, byte yuv[], int[] rgba, boolean debug);

    static {
        System.loadLibrary("objtrack_opencv_jni");
    }
}
