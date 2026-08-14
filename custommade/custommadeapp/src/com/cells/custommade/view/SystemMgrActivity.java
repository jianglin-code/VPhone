package com.cells.custommade.view;

import android.util.Log;
import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.WindowManager;
import android.os.SystemProperties;
import android.os.ServiceManager;
import android.os.RemoteException;
import android.widget.Toast;
import android.graphics.Color;
import android.widget.Button;
import android.widget.TextView;
import android.widget.EditText;
import android.widget.Switch;
import android.widget.CompoundButton;
import android.content.Intent;
import android.content.Context;
import android.net.Uri;
import java.io.InputStream;
import java.io.OutputStream;
import android.database.Cursor;
import android.provider.OpenableColumns;
import android.provider.MediaStore;
import android.provider.DocumentsContract;
import android.provider.Settings;
import android.os.Environment;
import android.content.ContentUris;
import android.content.SharedPreferences;
import android.telephony.TelephonyManager;

import com.cells.custommade.R;

import android.app.CellsPrivateServiceMgr;

import java.io.File;
import java.io.IOException;
import java.io.FileOutputStream;
import java.io.FileInputStream;
import java.nio.file.*;

import java.net.Socket;
import java.net.ServerSocket;
import java.net.InetSocketAddress;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import java.util.Random;

import java.lang.NumberFormatException;

public class SystemMgrActivity extends Activity {
	private static final String TAG = "CustommadeApp";
	private static final int REQUEST_CODE = 100;

	String props[] = {
		"persist.ro.custommade.lo.longitude",
		"persist.ro.custommade.lo.latitude",
		"persist.ro.custommade.lo.altitude",
		"persist.sys.camera.front.url",
		"persist.sys.camera.back.url",
		"persist.ro.custommade.deviceinfo.imei",
		"persist.ro.custommade.phone.imeisv",
		"persist.ro.custommade.phone.msinum",
		"persist.ro.custommade.deviceinfo.androidid",
		"persist.sys.camera.front.file",
		"persist.sys.camera.back.file",
		"persist.ro.custommade.phone.longname",
		"persist.ro.custommade.phone.shortname",
		"persist.ro.custommade.phone.mccmnc",
		"persist.sys.camera.loop",
		"persist.sys.camera.noaudio",
		"persist.sys.host.selinux.enforce",
		"persist.sys.custommade.prop",
		"persist.sys.custommade.uidroot",
		"persist.sys.custommade.partition.rw",
		"persist.sys.v4l2.camera.front",
		"persist.sys.v4l2.camera.back",
		"persist.ro.custommade.phone.sim",
		"persist.sys.camera.share.num",
	};

	String propdefvalues[] = {
		"103.82",
		"1.30",
		"0.0",
		"",
		"",
		"",
		"11",
		"",
		"",
		"/sdcard/Download/two1.jpg",
		"/sdcard/Download/one0.jpg",
		"中国移动",
		"CMCC",
		"46000",
		"0",
		"0",
		"0",
		"",
		"+10086",
		"0",
		"/dev/video33",
		"/dev/video34",
		"0",
		"0",
	};

	int gettextid(int index)
	{
		switch (index)
		{
			case 1: return R.id.edit1;
			case 2: return R.id.edit2;
			case 3: return R.id.edit3;
			case 4: return R.id.edit4;
			case 5: return R.id.edit5;
			case 6: return R.id.edit6;
			case 7: return R.id.edit7;
			case 8: return R.id.edit8;
			case 9: return R.id.edit9;
			case 10: return R.id.edit10;
			case 11: return R.id.edit11;
			case 12: return R.id.edit12;
			case 13: return R.id.edit13;
			case 14: return R.id.edit14;
			case 15: return R.id.edit15;
			case 16: return R.id.edit16;
			case 17: return R.id.edit17;
			case 18: return R.id.edit18;
			case 19: return R.id.edit19;
			case 20: return R.id.edit20;
			case 21: return R.id.edit21;
			case 22: return R.id.edit22;
			case 23: return R.id.edit23;
			case 24: return R.id.edit24;
			default: return R.id.edit1;
		}
	}

	private static final int MSG_RECEIVE_FINISHED            = 0x01;
	private Handler mHandler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			switch (msg.what)
			{
				case MSG_RECEIVE_FINISHED:
				{
					Button btn18 = (Button)findViewById(R.id.btn18);
					btn18.setClickable(true);
					btn18.setEnabled(true);

					//SystemProperties.set("persist.sys.custommade.prop", "");
					Toast.makeText(SystemMgrActivity.this, "Finished!", Toast.LENGTH_SHORT).show();
					break;
				}
			}
		}
	};

	public static String getIMEI(Context context) {
        try {
            TelephonyManager tm = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
            return tm.getDeviceId() != null ? tm.getDeviceId() : "";
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "";
	}
	public static String getPhoneNumber(Context context) {
        try {
            TelephonyManager tm = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
            return tm.getLine1Number() != null ? tm.getLine1Number() : "";
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "";
	}
	public static String getAndroidId(Context context) {
		return Settings.Secure.getString(context.getContentResolver(), Settings.Secure.ANDROID_ID);
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		setContentView(R.layout.activity_main);

		propdefvalues[5] = SystemMgrActivity.getIMEI(getApplicationContext());
		propdefvalues[7] = SystemMgrActivity.getPhoneNumber(getApplicationContext());
		propdefvalues[8] = SystemMgrActivity.getAndroidId(getApplicationContext());

		initCameraPic();

		getPropertys();

		initSwitch();

		enableBtns();
	}

	@Override
	public void onResume() {
		super.onResume();
	}

	void initSwitch()
	{
        Switch switch15 = (Switch) findViewById(R.id.edit15);
        switch15.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				setProperty(14);
			}
        });

        Switch switch16 = (Switch) findViewById(R.id.edit16);
        switch16.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				setProperty(15);
			}
        });

        Switch switch17 = (Switch) findViewById(R.id.edit17);
        switch17.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				setProperty(16);
				if (isChecked) {
					Switch switch20 = (Switch) findViewById(R.id.edit20);
					switch20.setEnabled(false);
				}
			}
        });

        Switch switch20 = (Switch) findViewById(R.id.edit20);
        switch20.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				setProperty(19);
				if (isChecked) {
					Switch switch17 = (Switch) findViewById(R.id.edit17);
					switch17.setEnabled(false);
				}
			}
        });

        Switch switch23 = (Switch) findViewById(R.id.edit23);
        switch23.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				setProperty(22);
			}
        });
	}

	void enableBtns()
	{
		String value = SystemProperties.get("persist.sys.host.selinux.enforce", "0");
		if (value.equals("1")) {
			Switch switch20 = (Switch) findViewById(R.id.edit20);
			switch20.setEnabled(false);
		}

		value = SystemProperties.get("persist.sys.custommade.partition.rw", "0");
		if (value.equals("1")) {
			Switch switch17 = (Switch) findViewById(R.id.edit17);
			switch17.setEnabled(false);
		}

		value = SystemProperties.get("ro.boot.vm", "0");
		if (value.equals("1")) {
			View layoutHost = findViewById(R.id.layouthost1);
			layoutHost.setVisibility(View.GONE);
		}
	}

	void getPropertys()
	{
        for (int i=0; i < props.length; i++) {
			String value = SystemProperties.get(props[i], propdefvalues[i]);

			View view = findViewById(gettextid(i+1));
			if (view instanceof EditText) {
				EditText editText = (EditText) view;
				editText.setText(value);
			} else if (view instanceof Switch) {
				Switch sw = (Switch) view;
				if (value.equals("0")) {
					sw.setChecked(false);
				} else {
					sw.setChecked(true);
				}
			}

			Log.d(TAG, "SystemProperty Read " + props[i] + " = " + value);
        }
	}

	void setProperty(int i)
	{
		View view = findViewById(gettextid(i+1));
		if (view == null) return;

		String value = "";

		if (view instanceof EditText) {
			EditText editText = (EditText) view;
			if (editText.getText().toString() != null && 
				editText.getText().toString().length() > 0)
				value = editText.getText().toString();
		} else if (view instanceof Switch) {
			Switch sw = (Switch) view;
			if (sw.isChecked()) {
				value = "1";
			} else {
				value = "0";
			}
		}

		String oldvalue = SystemProperties.get(props[i], propdefvalues[i]);
		//if (!value.equals(oldvalue)) 
		{
			SystemProperties.set(props[i], value);
			Log.d(TAG, "SystemProperty Write " + props[i] + " = " + value);

			String str = props[i] + " = " + value + " set success！";
			Toast.makeText(SystemMgrActivity.this, str, Toast.LENGTH_SHORT).show();
		}
	}

	public static boolean isNumeric(String str) {
		if (str == null || str.isEmpty()) {
			return false;
		}
		try {
			Double.parseDouble(str);
			return true;
		} catch (NumberFormatException e) {
			return false;
		}
	}
	public void btnClk1(View v)
	{
		EditText editText = findViewById(gettextid(1));
		if (editText == null)
			return ;
		
		String value = editText.getText().toString();
		if (value == null || value.length() == 0 || !isNumeric(value.trim()))
			return;

		double longitude = Double.parseDouble(value.trim());
		if (longitude >= -180 && longitude <= 180){
			setProperty(0);
		} else {
			String str = "longitude mast be -180 <= longitude <= 180";
			Toast.makeText(SystemMgrActivity.this, str, Toast.LENGTH_SHORT).show();
		}
	}
	public void btnClk2(View v)
	{
		EditText editText = findViewById(gettextid(2));
		if (editText == null)
			return ;
		
		String value = editText.getText().toString();
		if (value == null || value.length() == 0 || !isNumeric(value.trim()))
			return;

		double latitude = Double.parseDouble(value.trim());
		if (latitude >= -90 && latitude <= 90){
			setProperty(1);
		} else {
			String str = "latitude mast be -90 <= latitude <= 90";
			Toast.makeText(SystemMgrActivity.this, str, Toast.LENGTH_SHORT).show();
		}
	}
	public void btnClk3(View v)
	{
		EditText editText = findViewById(gettextid(3));
		if (editText == null)
			return ;
		
		String value = editText.getText().toString();
		if (value == null || value.length() == 0 || !isNumeric(value.trim()))
			return;

		setProperty(2);
	}
	public void btnClk4(View v)
	{
		setProperty(3);
	}
	public void btnClk41(View v)
	{
		if (cureditid != -1)
			return;

		cureditid = 4;

		Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
		intent.addCategory(Intent.CATEGORY_OPENABLE);
		intent.setType("image/* video/*");
		String[] mimeTypes = {"image/*", "video/*"};
		intent.putExtra(Intent.EXTRA_MIME_TYPES, mimeTypes);
		startActivityForResult(intent, REQUEST_CODE);
	}

	public void btnClk5(View v)
	{
		setProperty(4);
	}
	public void btnClk51(View v)
	{
		if (cureditid != -1)
			return;

		cureditid = 5;

		Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
		intent.addCategory(Intent.CATEGORY_OPENABLE);
		intent.setType("image/* video/*");
		String[] mimeTypes = {"image/*", "video/*"};
		intent.putExtra(Intent.EXTRA_MIME_TYPES, mimeTypes);
		startActivityForResult(intent, REQUEST_CODE);
	}

	public void btnClk6(View v)
	{
		setProperty(5);
	}
	public void btnClk61(View v)
	{
		String str = SystemMgrActivity.generateIMEI();
		TextView textView = findViewById(gettextid(6));
		if (textView != null) textView.setText(str);
		setProperty(5);
	}
	public void btnClk7(View v)
	{
		setProperty(6);
	}
	public void btnClk8(View v)
	{
		setProperty(7);
	}
	public void btnClk9(View v)
	{
		setProperty(8);
	}
	public void btnClk91(View v)
	{
		String str = SystemMgrActivity.generateAndroidId();
		TextView textView = findViewById(gettextid(9));
		if (textView != null) textView.setText(str);
		setProperty(8);
	}
	public void btnClk10(View v)
	{
		setProperty(9);
	}
	public void btnClk101(View v)
	{
		if (cureditid != -1)
			return;

		cureditid = 10;

		Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
		intent.addCategory(Intent.CATEGORY_OPENABLE);
		intent.setType("image/* video/*");
		String[] mimeTypes = {"image/*", "video/*"};
		intent.putExtra(Intent.EXTRA_MIME_TYPES, mimeTypes);
		startActivityForResult(intent, REQUEST_CODE);
	}
	public void btnClk11(View v)
	{
		setProperty(10);
	}
	public void btnClk111(View v)
	{
		if (cureditid != -1)
			return;

		cureditid = 11;

		Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
		intent.addCategory(Intent.CATEGORY_OPENABLE);
		intent.setType("image/* video/*");
		String[] mimeTypes = {"image/*", "video/*"};
		intent.putExtra(Intent.EXTRA_MIME_TYPES, mimeTypes);
		startActivityForResult(intent, REQUEST_CODE);
	}
	public void btnClk12(View v)
	{
		setProperty(11);
	}
	public void btnClk13(View v)
	{
		setProperty(12);
	}
	public void btnClk14(View v)
	{
		setProperty(13);
	}
	public void btnClk15(View v)
	{
		setProperty(14);
	}
	public void btnClk16(View v)
	{
		setProperty(15);
	}
	public void btnClk17(View v)
	{
		setProperty(16);
	}
	public void btnClk18(View v)
	{
		//setProperty(17);
		Button btn18 = (Button)findViewById(R.id.btn18);
		btn18.setClickable(false);
		btn18.setEnabled(false);

		EditText editText = (EditText) findViewById(gettextid(17+1));
		if (editText == null)
			return;

		String serverIp = editText.getText().toString();
		if (serverIp == null || serverIp.length() == 0)
			return;

		new Thread(new Runnable() {
			@Override
			public void run() {
				try {
					Socket socket = new Socket(serverIp, 8908);
					InputStream is = socket.getInputStream();

					String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss", Locale.getDefault()).format(new Date());
					File file = new File(Environment.getExternalStorageDirectory(), timeStamp + ".prop");

					FileOutputStream fos = new FileOutputStream(file);

					byte[] buffer = new byte[4096];
					int len;
					while ((len = is.read(buffer)) != -1) {
						fos.write(buffer, 0, len);
					}

					fos.flush();

					SystemProperties.set("persist.sys.custommade.prop", file.getAbsolutePath());
					Log.d(TAG, "File received successfully");
				} catch (IOException e) {
					Log.e(TAG, "File received failed");
					e.printStackTrace();
				}

				mHandler.sendMessage(Message.obtain(mHandler, MSG_RECEIVE_FINISHED));
			}
		}).start();
	}
	public void btnClk19(View v)
	{
		setProperty(18);
	}
	public void btnClk20(View v)
	{
		setProperty(19);
	}

	public void btnClk21(View v)
	{
		setProperty(20);
	}
	public void btnClk22(View v)
	{
		setProperty(21);
	}
	public void btnClk23(View v)
	{
		setProperty(22);
	}
	public void btnClk24(View v)
	{
		setProperty(23);
	}

	int cureditid = -1;
	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		if (requestCode == REQUEST_CODE && resultCode == RESULT_OK && data != null) {
			Uri uri = data.getData();
			if (uri != null && cureditid != -1) {
				Log.d(TAG, "uri: " + uri); //  content://com.android.providers.media.documents/document/
				String absolutePath = getImageAbsolutePath(SystemMgrActivity.this, uri);

				Log.d(TAG, "absolutePath: " + absolutePath);
				TextView textView = findViewById(gettextid(cureditid));
				if (textView != null)
					textView.setText(absolutePath);
			}
		}

		cureditid = -1;
	}

    public static String getImageAbsolutePath(Activity context, Uri uri) {
        if (context == null || uri == null)
            return null;
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.KITKAT && 
			DocumentsContract.isDocumentUri(context, uri)) {
            if (isExternalStorageDocument(uri)) {
                String docId = DocumentsContract.getDocumentId(uri);
                String[] split = docId.split(":");
                String type = split[0];
                if ("primary".equalsIgnoreCase(type)) {
                    return Environment.getExternalStorageDirectory() + "/" + split[1];
                }
            } else if (isDownloadsDocument(uri)) {
                String docId = DocumentsContract.getDocumentId(uri);

				if (docId != null && docId.startsWith("raw:")) {
					return docId.substring(4);
				}

				if (docId.startsWith("msf:")) {
					String[] split = docId.split(":");
					String type = split[0];
					Uri mediaUri = null;
					if ("image".equals(type)) {
						mediaUri = MediaStore.Images.Media.EXTERNAL_CONTENT_URI;
					} else if ("video".equals(type)) {
						mediaUri = MediaStore.Video.Media.EXTERNAL_CONTENT_URI;
					} else if ("audio".equals(type)) {
						mediaUri = MediaStore.Audio.Media.EXTERNAL_CONTENT_URI;
					} else {
						mediaUri = MediaStore.Files.getContentUri("external");
					}
					return getDataColumn(context, mediaUri, MediaStore.Files.FileColumns._ID + "=?", new String[]{split[1]});
				}

                Uri contentUri = ContentUris.withAppendedId(Uri.parse("content://downloads/public_downloads"), Long.parseLong(docId));
                return getDataColumn(context, contentUri, null, null);
            } else if (isMediaDocument(uri)) {
                String docId = DocumentsContract.getDocumentId(uri);
                String[] split = docId.split(":");
                String type = split[0];
                Uri mediaUri = null;
                if ("image".equals(type)) {
                    mediaUri = MediaStore.Images.Media.EXTERNAL_CONTENT_URI;
                } else if ("video".equals(type)) {
                    mediaUri = MediaStore.Video.Media.EXTERNAL_CONTENT_URI;
                } else if ("audio".equals(type)) {
                    mediaUri = MediaStore.Audio.Media.EXTERNAL_CONTENT_URI;
                } else {
					mediaUri = MediaStore.Files.getContentUri("external");
				}
                return getDataColumn(context, mediaUri, MediaStore.Files.FileColumns._ID + "=?", new String[]{split[1]});
            }
        } // MediaStore (and general)
        else if ("content".equalsIgnoreCase(uri.getScheme())) {
            // Return the remote address
            if (isGooglePhotosUri(uri))
                return uri.getLastPathSegment();
            return getDataColumn(context, uri, null, null);
        }
        // File
        else if ("file".equalsIgnoreCase(uri.getScheme())) {
            return uri.getPath();
        }
        return null;
    }
    public static String getMediaTypeFromMediaStore(Context context, Uri uri) {
        String[] projection = {MediaStore.Files.FileColumns.MEDIA_TYPE};
        try (Cursor cursor = context.getContentResolver().query(uri, projection, null, null, null)) {
            if (cursor != null && cursor.moveToFirst()) {
                int mediaType = cursor.getInt(0);
                if (mediaType == MediaStore.Files.FileColumns.MEDIA_TYPE_IMAGE) {
                    return MediaStore.Images.Media.DATA;
                } else if (mediaType == MediaStore.Files.FileColumns.MEDIA_TYPE_VIDEO) {
                    return MediaStore.Video.Media.DATA;
                }
            }
        }
        return MediaStore.Images.Media.DATA;
    }
    public static String getDataColumn(Context context, Uri uri, String selection, String[] selectionArgs) {
        Cursor cursor = null;
        String column = MediaStore.Images.Media.DATA;
        String[] projection = {column};  //告诉Provider要返回的内容（列Column）
		Log.d(TAG, "getDataColumn: " + column);
        try {
            cursor = context.getContentResolver().query(uri, projection, selection, selectionArgs, null);
            if (cursor != null && cursor.moveToFirst()) {
                int index = cursor.getColumnIndexOrThrow(column);
                return cursor.getString(index);
            }
        } finally {
            if (cursor != null)
                cursor.close();
        }
        return null;
    }
    public static boolean isExternalStorageDocument(Uri uri) {
        return "com.android.externalstorage.documents".equals(uri.getAuthority());
    }
    public static boolean isDownloadsDocument(Uri uri) {
        return "com.android.providers.downloads.documents".equals(uri.getAuthority());
    }
    public static boolean isMediaDocument(Uri uri) {
        return "com.android.providers.media.documents".equals(uri.getAuthority());
    }
    public static boolean isGooglePhotosUri(Uri uri) {
        return "com.google.android.apps.photos.content".equals(uri.getAuthority());
    }

	private void initCameraPic()
	{
        SharedPreferences prefs = getSharedPreferences("AppPrefs", MODE_PRIVATE);
        boolean isFirstRun = prefs.getBoolean("isFirstRun", true);
        if (isFirstRun)
		{
            Log.d(TAG, "initCameraPic isFirstRun");
            prefs.edit().putBoolean("isFirstRun", false).apply();

			Path path = Paths.get("/sdcard/Download/one0.jpg");
			if (!Files.exists(path)) {
				InputStream p=getResources().openRawResource(R.raw.one0);
				copyFile(p,"/sdcard/Download/one0.jpg");
				scanFile(SystemMgrActivity.this,"/sdcard/Download/one0.jpg");
				if (SystemProperties.get("persist.sys.camera.back.file", "").equals("")) {
					SystemProperties.set("persist.sys.camera.back.file", "/sdcard/Download/one0.jpg");
				}
			}

			Path path1 = Paths.get("/sdcard/Download/two1.jpg");
			if (!Files.exists(path1)) {
				InputStream p=getResources().openRawResource(R.raw.two1);
				copyFile(p,"/sdcard/Download/two1.jpg");
				scanFile(SystemMgrActivity.this,"/sdcard/Download/two1.jpg");
				if (SystemProperties.get("persist.sys.camera.front.file", "").equals("")) {
					SystemProperties.set("persist.sys.camera.front.file", "/sdcard/Download/two1.jpg");
				}
			}

			//SystemProperties.set("persist.ro.custommade.deviceinfo.imei", SystemMgrActivity.generateIMEI());
			//SystemProperties.set("persist.ro.custommade.deviceinfo.androidid", SystemMgrActivity.generateAndroidId());
        }
	}

	private boolean copyFile(InputStream fis,String des){
		boolean result=true;
		FileOutputStream fos=null;
		
		try{
			File fout=new File(des);
			if(!fout.exists()){
				fos=new FileOutputStream(fout);
				byte[] b=new byte[1024];
				int len;
				while((len=fis.read(b))!=-1){
					fos.write(b, 0, len);
				}
			}
		}catch(IOException e){
			e.printStackTrace();
			result=false;
		}finally{
			if(fos!=null){
				try{
					fos.close();
				}catch(IOException e){
					e.printStackTrace();
					result=false;
				}
			}
			if(fis!=null){
				try{
					fis.close();
				}catch(IOException e){
					e.printStackTrace();
					result=false;
				}
			}
		}
		return result;
	}
	private void scanFile(final Context context,final String path) {
		Intent scanIntent=new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE);
		scanIntent.setData(Uri.fromFile(new File(path)));
		context.sendBroadcast(scanIntent);
	}

    public static String generateIMEI() {
        String tac = "86838703";
        Random random = new Random();
        int snr = 100000 + random.nextInt(900000); // 6-digit serial number
        String first14 = tac + snr;
        int checkDigit = calculateLuhn(first14);
        return first14 + checkDigit;
    }
    public static int calculateLuhn(String imeiWithoutCheckDigit) {
        int sum = 0;
        for (int i = 0; i < imeiWithoutCheckDigit.length(); i++) {
            int digit = Character.getNumericValue(imeiWithoutCheckDigit.charAt(i));
            if (i % 2 == 1) {
                digit *= 2;
                if (digit > 9) digit -= 9;
            }
            sum += digit;
        }
        return (10 - (sum % 10)) % 10;
    }

    public static String generateAndroidId() {
        Random random = new Random();
        StringBuilder androidId = new StringBuilder();
        for (int i = 0; i < 16; i++) {
            int n = random.nextInt(16); // 0~15
            androidId.append(Integer.toHexString(n));
        }
        return androidId.toString();
    }
}
