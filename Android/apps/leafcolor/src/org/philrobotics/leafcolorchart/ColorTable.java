package org.philrobotics.leafcolorchart;

import org.opencv.core.Scalar;

import android.util.Log;

public class ColorTable {
	
	// leaf/green hsv table
    private int[]                mGreenHues;
    private int[]                mGreenSats;
    private int[]                mGreenVals;
    
    public ColorTable(){
    	mGreenHues = new int[] {
				38, 39, 40, 41, 42, 43, 45, 46, 47, 48,
				49, 50, 51, 52, 53, 54, 55, 56, 58, 59,
				60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
				71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
				81, 82, 83, 85, 86, 87, 88, 89, 90, 91,
				92, 93, 94, 95, 96, 98, 99, 100, 101, 102 };
        mGreenSats = new int[] {
				88, 87, 87, 87, 87, 87, 86, 86, 86, 86,
				85, 85, 85, 85, 84, 84, 84, 84, 83, 83,
				83, 83, 82, 82, 82, 82, 81, 81, 81, 81,
				80, 80, 80, 80, 79, 79, 79, 79, 78, 78,
				78, 78, 77, 77, 77, 77, 76, 76, 76, 76,
				76, 75, 75, 75, 75, 74, 74, 74, 74, 73 };
        mGreenVals = new int[] {
				115, 114, 114, 113, 113, 112, 112, 112, 111, 111,
				110, 110, 109, 109, 108, 108, 107, 107, 106, 106,
				105, 105, 104, 104, 104, 103, 103, 102, 102, 101,
				101, 100, 100, 99, 99, 98, 98, 97, 97, 97,
				96, 96, 95, 95, 94, 94, 93, 93, 92, 92,
				91, 91, 90, 90, 89, 89, 89, 88, 88, 87 };
    }
    
    public int lookup(int bH, int bS, int bV){
    	int ret = -1;
    	
    	if( bV < 20 ) return -1; // black
    	
    	if( bH<(mGreenHues[0]-10) || bH>(mGreenHues[mGreenHues.length-1]+10) ) return -1; 
    	
    	// Color matching
		double near = 200;//128;		
		for(int i=0; i<mGreenHues.length; i++){    		
    		double distance = (bH-mGreenHues[i]) * (bH-mGreenHues[i]);
    		distance += ( (bS-mGreenSats[i]) * (bS-mGreenSats[i]) );
    		distance += ( (bV-mGreenVals[i]) * (bV-mGreenVals[i]) );
    		distance = Math.sqrt(distance);
    		
    		if( distance < near ){
    			near = distance;
    			ret = i;
    		}
    	}
    	
    	return ret;    	
    }
    
    public int getLength(){
    	return mGreenHues.length;
    }
    
    
    public void calibrate(Scalar hsv2, Scalar hsv5){
    	Log.i("cal", String.format("(%.0f,%.0f,%.0f) - (%.0f,%.0f,%.0f)",
    			hsv2.val[0], hsv2.val[1], hsv2.val[2],
    			hsv5.val[0], hsv5.val[1], hsv5.val[2]));
    	
    	double Hslope = (hsv5.val[0] - hsv2.val[0]) / (44 - 15); 
    	double Sslope = (hsv5.val[1] - hsv2.val[1]) / (44 - 15);
    	double Vslope = (hsv5.val[2] - hsv2.val[2]) / (44 - 15);
    	
    	//double BSlope
    	//y = m*x + b
    	//hsv5.val[0] = Hslope * 15 + b
    	double Hb = hsv2.val[0] - (Hslope*15); 
    	double Sb = hsv2.val[1] - (Sslope*15);
    	double Vb = hsv2.val[1] - (Vslope*15);
    	
    	for(int i=0; i<mGreenHues.length; i++){
    		mGreenHues[i] = (int)(Hslope*i + Hb);
    		mGreenSats[i] = (int)(Sslope*i + Sb);
    		mGreenVals[i] = (int)(Vslope*i + Vb);    		
    	}
    	
    }
    
}
