/***************************************
 * 
 * Android Bluetooth Dual Joystick
 * yus - projectproto.blogspot.com
 * February 2012
 *  
 ***************************************/

package org.projectproto.btjoystick;

import com.MobileAnarchy.Android.Widgets.Joystick.DualJoystickView;
import com.MobileAnarchy.Android.Widgets.Joystick.JoystickMovedListener;
import org.projectproto.btjoystick.DeviceListActivity;
import org.projectproto.btjoystick.BluetoothRfcommClient;

import java.util.Timer;
import java.util.TimerTask;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class BluetoothJoystickActivity extends Activity {
	
	// debug / logs
    private final boolean D = false;
    private static final String TAG = BluetoothJoystickActivity.class.getSimpleName();
    
	// Message types sent from the BluetoothRfcommClient Handler
    public static final int MESSAGE_STATE_CHANGE = 1;
    public static final int MESSAGE_READ = 2;
    public static final int MESSAGE_WRITE = 3;
    public static final int MESSAGE_DEVICE_NAME = 4;
    public static final int MESSAGE_TOAST = 5;
    
    // Key names received from the BluetoothRfcommClient Handler
    public static final String DEVICE_NAME = "device_name";
    public static final String TOAST = "toast";
	
	// Intent request codes
    private static final int REQUEST_CONNECT_DEVICE = 1;
    private static final int REQUEST_ENABLE_BT = 2;
	
    // Name of the connected device
    private String mConnectedDeviceName = null;
    // Local Bluetooth adapter
    private BluetoothAdapter mBluetoothAdapter = null;
    // Member object for the RFCOMM services
    private BluetoothRfcommClient mRfcommClient = null;
    
	// Layout View
 	DualJoystickView mDualJoystick;
 	private Button mConnectButton;
 	private TextView mTxtStatus;
 	private TextView mTxtDataL;
 	private TextView mTxtDataR;
 	
 	// polar coordinates
 	private double mRadiusL = 0, mRadiusR = 0;
 	private double mAngleL = 0, mAngleR = 0;
 	private boolean mCenterL = true, mCenterR = true;
 	
 	private Timer mUpdateTimer;
 	private int mTimeoutCounter = 0;
 	private static final int MaxTimeoutCount = 15; // actual timeout = count * updateperiod 
 	private static final long UpdatePeriod = 200; // in milliseconds 	
 	private static final byte  DATA_START = 0x55;
	
 	/** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        // Get local Bluetooth adapter
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        
        // If the adapter is null, then Bluetooth is not supported
        if (mBluetoothAdapter == null) {
            Toast.makeText(this, "Bluetooth is not available", Toast.LENGTH_LONG).show();
            finish();
            return;
        }
        
        // If BT is not on, request that it be enabled.
    	if (!mBluetoothAdapter.isEnabled()){
    		Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
    		startActivityForResult(enableIntent, REQUEST_ENABLE_BT);
    	}
    	
    	// Initialize the BluetoothRfcommClient to perform bluetooth connections
        mRfcommClient = new BluetoothRfcommClient(this, mHandler);
        
        mDualJoystick = (DualJoystickView)findViewById(R.id.dualjoystickView);
        mDualJoystick.setOnJostickMovedListener(_listenerLeft, _listenerRight);
        // mDualJoystick.setYAxisInverted(false, false);
        
        mTxtStatus = (TextView) findViewById(R.id.txt_status);
        mTxtDataL = (TextView) findViewById(R.id.txt_dataL);
        mTxtDataR = (TextView) findViewById(R.id.txt_dataR);
        
        mConnectButton = (Button) findViewById(R.id.button_connect);
        mConnectButton.setOnClickListener(new OnClickListener() {
        	public void onClick(View arg0) {
        		BTConnect();
        	}
        });
        
        mUpdateTimer = new Timer();
        mUpdateTimer.schedule(new TimerTask() {
				@Override
				public void run() {
					UpdateMethod();
				}
			}, 2000, UpdatePeriod);
    }
    
    @Override
    public synchronized void onResume() {
    	super.onResume();
    	if (mRfcommClient != null) {
            // Only if the state is STATE_NONE, do we know that we haven't started already
            if (mRfcommClient.getState() == BluetoothRfcommClient.STATE_NONE) {
              // Start the Bluetooth  RFCOMM services
              mRfcommClient.start();
            }
        }    	
    }
    
    @Override
    public void onDestroy() {
    	super.onDestroy();
    	// Stop the Bluetooth RFCOMM services
        if (mRfcommClient != null) mRfcommClient.stop();
        mUpdateTimer.cancel();
    }
    
    private JoystickMovedListener _listenerLeft = new JoystickMovedListener() {
    	
    	@Override
		public void OnMoved(int pan, int tilt) {
    		mRadiusL = Math.sqrt((pan*pan) + (tilt*tilt));
    		// mAngleL = Math.atan2(pan, tilt);
    		mAngleL = Math.atan2(-pan, -tilt);
    		mTxtDataL.setText(String.format("( r%.0f, %.0f\u00B0 )", Math.min(mRadiusL, 10), mAngleL * 180 / Math.PI));
    		mCenterL = false;
    	}
    	
    	@Override
		public void OnReleased() {
    		// 
    	}
    	
    	public void OnReturnedToCenter() {
    		mCenterL = true;
    		mRadiusL = 0;
    		mAngleL = 0;
    	}
    };
    
    private JoystickMovedListener _listenerRight = new JoystickMovedListener() {
    	
    	@Override
		public void OnMoved(int pan, int tilt) {
    		mRadiusR = Math.sqrt((pan*pan) + (tilt*tilt));
    		// mAngleR = Math.atan2(pan, tilt);
    		mAngleR = Math.atan2(-pan, -tilt);
    		mTxtDataR.setText(String.format("( r%.0f, %.0f\u00B0 )", Math.min(mRadiusR, 10), mAngleR * 180 / Math.PI ));
    		mCenterR = false;
    	}
    	
    	@Override
		public void OnReleased() {
    		//
    	}
    	
    	public void OnReturnedToCenter() {
    		mCenterR = true;
    		mRadiusR = 0;
    		mAngleR = 0;
    	}
    };
    
    private void BTConnect(){
    	Intent serverIntent = new Intent(this, DeviceListActivity.class);
    	startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE);
    }
    
    /**
     * Sends a message.
     * @param message  A string of text to send.
     */
    private void sendMessage(String message){
    	// Check that we're actually connected before trying anything
    	if (mRfcommClient.getState() != BluetoothRfcommClient.STATE_CONNECTED) {
    		// Toast.makeText(this, R.string.not_connected, Toast.LENGTH_SHORT).show();
    		return;
    	}
    	// Check that there's actually something to send
    	if (message.length() > 0) {
    		// Get the message bytes and tell the BluetoothRfcommClient to write
    		byte[] send = message.getBytes();
    		mRfcommClient.write(send);
    	}
    }
    
    private void UpdateMethod() {
    	
    	// if either of the joysticks is not on the center, or timeout occurred
    	if(!mCenterL || !mCenterR || mTimeoutCounter>=MaxTimeoutCount) {
    		// limit to {0..10}
	    	byte radiusL = (byte) ( Math.min( mRadiusL, 10.0 ) );
	    	byte radiusR = (byte) ( Math.min( mRadiusR, 10.0 ) );
    		// scale to {0..35}
	    	byte angleL = (byte) ( mAngleL * 18.0 / Math.PI + 36.0 + 0.5 );
	    	byte angleR = (byte) ( mAngleR * 18.0 / Math.PI + 36.0 + 0.5 );	    		    	
	    	if( angleL >= 36 )	angleL = (byte)(angleL-36);
	    	if( angleR >= 36 )	angleR = (byte)(angleR-36);
	    	
	    	if (D) {
	    		Log.d(TAG, String.format("%d, %d, %d, %d, %d",
	    				DATA_START, radiusL, angleL, radiusR, angleR ) );
	    	}
	    	
	    	sendMessage( new String(new byte[] {
	    			DATA_START, radiusL, angleL, radiusR, angleR } ) );
	    	
	    	mTimeoutCounter = 0;
    	}
    	else{
    		mTimeoutCounter++;
    	}	
    }
    
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
    	switch (requestCode){
    	case REQUEST_CONNECT_DEVICE:
    		// When DeviceListActivity returns with a device to connect
    		if (resultCode == Activity.RESULT_OK) {
    			// Get the device MAC address
    			String address = data.getExtras().getString(DeviceListActivity.EXTRA_DEVICE_ADDRESS);
    			// Get the BLuetoothDevice object
                BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);
                // Attempt to connect to the device
                mRfcommClient.connect(device);
    		}
    		break;
    	case REQUEST_ENABLE_BT:
    		// When the request to enable Bluetooth returns
    		if (resultCode != Activity.RESULT_OK) {
            	// User did not enable Bluetooth or an error occured
                Toast.makeText(this, R.string.bt_not_enabled_leaving, Toast.LENGTH_SHORT).show();
                finish();
            }
    		break;
    	}
    }
    
 // The Handler that gets information back from the BluetoothRfcommClient
    private final Handler mHandler = new Handler() {
    	@Override
        public void handleMessage(Message msg) {
    		switch (msg.what) {
    		case MESSAGE_STATE_CHANGE:
    			switch (msg.arg1) {
    			case BluetoothRfcommClient.STATE_CONNECTED:
    				mTxtStatus.setText(R.string.title_connected_to);
    				mTxtStatus.append(" " + mConnectedDeviceName);
    				break;
    			case BluetoothRfcommClient.STATE_CONNECTING:
    				mTxtStatus.setText(R.string.title_connecting);
    				break;
    			case BluetoothRfcommClient.STATE_NONE:
    				mTxtStatus.setText(R.string.title_not_connected);
    				break;
    			}
    			break;
    		case MESSAGE_READ:
    			// byte[] readBuf = (byte[]) msg.obj;
    			// int data_length = msg.arg1;
    			break;
    		case MESSAGE_DEVICE_NAME:
    			// save the connected device's name
                mConnectedDeviceName = msg.getData().getString(DEVICE_NAME);
                Toast.makeText(getApplicationContext(), "Connected to "
                        + mConnectedDeviceName, Toast.LENGTH_SHORT).show();
    			break;
    		case MESSAGE_TOAST:
    			Toast.makeText(getApplicationContext(), msg.getData().getString(TOAST),
                        Toast.LENGTH_SHORT).show();
    			break;
    		}
    	}
    };
}