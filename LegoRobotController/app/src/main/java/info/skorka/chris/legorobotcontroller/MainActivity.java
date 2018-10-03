package info.skorka.chris.legorobotcontroller;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.support.v7.app.ActionBar;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.os.Handler;
import android.view.MotionEvent;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.SeekBar;
import android.widget.Spinner;
import android.widget.TextView;

import org.w3c.dom.Text;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

/**
 * An example full-screen activity that shows and hides the system UI (i.e.
 * status bar and navigation/system bar) with user interaction.
 */
public class MainActivity extends AppCompatActivity {

    /**
     * Some older devices needs a small delay between UI widget updates
     * and a change of the status and navigation bar.
     */
    private static final int UI_ANIMATION_DELAY = 300;
    private final Handler mHideHandler = new Handler();
    private View mContentView;
    private final Runnable mHidePart2Runnable = new Runnable() {
        @SuppressLint("InlinedApi")
        @Override
        public void run() {
            // Delayed removal of status and navigation bar

            // Note that some of these constants are new as of API 16 (Jelly Bean)
            // and API 19 (KitKat). It is safe to use them, as they are inlined
            // at compile-time and do nothing on earlier devices.
            mContentView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_LOW_PROFILE
                    | View.SYSTEM_UI_FLAG_FULLSCREEN
                    | View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                    | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                    | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                    | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);
        }
    };

    private String btName = "HC-06";
    private BluetoothAdapter btAdapter;
    private BluetoothDevice btDeviceLegoRobot;
    private BluetoothSocket btSocket;
    private OutputStream btOutputStream;
    private InputStream btInputStream;

    private SeekBar seekBar1, seekBar2;
    private TextView text1, text2;

    private enum Mode {MANUAL, DIRECT, HEADING};
    private String modeNames[] = {"Manual", "Direct", "Heading"};

    private Mode mode = Mode.MANUAL;

    private int speed = 0;
    private int steering = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        mContentView = findViewById(R.id.main_view);

        // get mode selector reference
        // Spinner modeSpinner = findViewById(R.id.mode_spinner);
        // ArrayAdapter<String> spinnerArrayAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, modeNames);
        // spinnerArrayAdapter.setDropDownViewResource(android.R.layout
        //         .simple_spinner_dropdown_item);
        // modeSpinner.setAdapter(spinnerArrayAdapter);

        // get input references
        seekBar1 = findViewById(R.id.seekbar1);
        seekBar2 = findViewById(R.id.seekbar2);

        // get text outputs
        text1 = findViewById(R.id.text1);
        text2 = findViewById(R.id.text2);

        // setup relevant callbacks
        seekBar1.setOnSeekBarChangeListener(onSeekBar1Change);
        seekBar2.setOnSeekBarChangeListener(onSeekBar2Change);
        // modeSpinner.setOnItemSelectedListener(onModeSpinnerChange);

        // setup up bluetooth
        btAdapter = BluetoothAdapter.getDefaultAdapter();

        // set default values
        stop(null);

    }

    SeekBar.OnSeekBarChangeListener onSeekBar1Change = new SeekBar.OnSeekBarChangeListener() {
        @Override
        public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
            text1.setText("Speed: " + String.valueOf(progress));
            sendSpeedCommand(progress);
        }

        @Override
        public void onStartTrackingTouch(SeekBar seekBar) {

        }

        @Override
        public void onStopTrackingTouch(SeekBar seekBar) {

        }
    };

    SeekBar.OnSeekBarChangeListener onSeekBar2Change = new SeekBar.OnSeekBarChangeListener() {
        @Override
        public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
            text2.setText("Heading: " + String.valueOf(progress) + "°");
            sendDirectionCommand(progress);
        }

        @Override
        public void onStartTrackingTouch(SeekBar seekBar) {

        }

        @Override
        public void onStopTrackingTouch(SeekBar seekBar) {

        }
    };

    AdapterView.OnItemSelectedListener onModeSpinnerChange = new AdapterView.OnItemSelectedListener() {
        @Override
        public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
            System.out.println(position);
        }

        @Override
        public void onNothingSelected(AdapterView<?> parent) {

        }
    };

    @Override
    protected void onPostCreate(Bundle savedInstanceState) {
        super.onPostCreate(savedInstanceState);

        // Trigger the initial hide() shortly after the activity has been
        // created, to briefly hint to the user that UI controls
        // are available.
        // delayedHide(0);
        hide();
    }

    @Override
    protected void onStart() {
        super.onStart();

        hide();
        bluetoothConnect(btName);
    }

    @Override
    protected void onStop() {
        super.onStop();

        bluetoothDisconnect();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        bluetoothDisconnect();
    }

    private void hide() {
        // Hide UI first
        ActionBar actionBar = getSupportActionBar();
        if (actionBar != null) {
            actionBar.hide();
        }
        // mControlsView.setVisibility(View.GONE);
        // mVisible = false;

        // Schedule a runnable to remove the status and navigation bar after a delay
        mHideHandler.postDelayed(mHidePart2Runnable, UI_ANIMATION_DELAY);
    }

    // connect to the device
    private void bluetoothConnect(String name){
        Set<BluetoothDevice> pairedDevices = btAdapter.getBondedDevices();
        BluetoothDevice btDeviceLegoRobot = null;

        for(BluetoothDevice btDevice : pairedDevices){
            if(btDevice.getName().equals(name))
                btDeviceLegoRobot = btDevice;
        }

        if(btDeviceLegoRobot != null) {
            UUID uuid = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"); // Standard SerialPortService ID

            try {

                btSocket = btDeviceLegoRobot.createRfcommSocketToServiceRecord(uuid);
                btSocket.connect();
                btOutputStream = btSocket.getOutputStream();
                btInputStream = btSocket.getInputStream();

            } catch (IOException e) {
                e.printStackTrace();
            }

        }

    }

    // send data to bluetooth device
    void bluetoothSend(byte c){
        try {
            btOutputStream.write(c);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    // disconnect from bluetooth device
    void bluetoothDisconnect() {
        try {
            btOutputStream.close();
            btInputStream.close();
            btSocket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    // generate manual command and transmit it
    // manual mode is no longer used
    void generateCommand(){

        byte command = 0;

        byte st = (byte) Math.round(steering / 30);
        byte sp = (byte) Math.round(-speed / 14);

        command |= st & 0x07;
        command |= (sp & 0x0F) << 3;

        System.out.println(Integer.toBinaryString(command));

        bluetoothSend(command);
    }

    // send speed command
    void sendSpeedCommand(int speed){

        byte command = -0x80;
        byte s = (byte) -speed;
        command |= s & 0x0F;

        bluetoothSend(command);
    }

    // send direction command
    void sendDirectionCommand(int direction){

        byte command = 0x00;
        byte d = (byte) Math.round(direction / 3);
        command |= d & 0x7F;

        bluetoothSend(command);
    }

    // stop and reset
    void stop(View view){
        seekBar1.setProgress(0);
        seekBar2.setProgress(0);
    }
}
