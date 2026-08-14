package com.cells.systemmgr.view;

import android.util.Log;
import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.WindowManager;
import android.os.SystemProperties;
import android.os.ServiceManager;
import java.io.IOException;
import android.os.RemoteException;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileInputStream;
import android.widget.Toast;
import android.graphics.Color;
import android.widget.Button;
import android.widget.TextView;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.res.Configuration;
import android.content.Context;

import com.cells.systemmgr.R;

import android.app.CellsPrivateServiceMgr;

public class SystemMgrActivity extends Activity {
	private static final String TAG = "CellsApp";

	private static final int MSG_START_STOP_VM            = 0x01;
	private static final int MSG_GET_VMLIST  	          = 0x02;
	private static final int MSG_START_VM_TOAST_CODE_SUCC = 0x03;
	private static final int MSG_START_VM_TOAST_CODE_ERR  = 0x04;
	private static final int MSG_RESET_VM                 = 0x05;
	private static final int MSG_BACKUP_VM                = 0x06;
	private static final int MSG_RESTORE_VM               = 0x07;
	private static final int MSG_BACKUP_VM_TOAST_CODE_SUCC   = 0x08;
	private static final int MSG_BACKUP_VM_TOAST_CODE_ERR    = 0x09;
	private static final int MSG_RESTORE_VM_TOAST_CODE_SUCC = 0x0a;
	private static final int MSG_RESTORE_VM_TOAST_CODE_ERR  = 0x0b;

	private String hostip = "";

	private Handler mHandler = new Handler() {

		int getbtnid(int index)
		{
			switch (index)
			{
				case 1: return R.id.btn_cell1;
				case 2: return R.id.btn_cell2;
				case 3: return R.id.btn_cell3;
				case 4: return R.id.btn_cell4;
				case 5: return R.id.btn_cell5;
				case 6: return R.id.btn_cell6;
				case 7: return R.id.btn_cell7;
				case 8: return R.id.btn_cell8;
				case 9: return R.id.btn_cell9;
				case 10: return R.id.btn_cell10;
				default: return R.id.btn_cell1;
			}
		}

		int gettextid(int index)
		{
			switch (index)
			{
				case 1: return R.id.text_cell1;
				case 2: return R.id.text_cell2;
				case 3: return R.id.text_cell3;
				case 4: return R.id.text_cell4;
				case 5: return R.id.text_cell5;
				case 6: return R.id.text_cell6;
				case 7: return R.id.text_cell7;
				case 8: return R.id.text_cell8;
				case 9: return R.id.text_cell9;
				case 10: return R.id.text_cell10;
				default: return R.id.text_cell1;
			}
		}

		int getresetbtnid(int index)
		{
			switch (index)
			{
				case 1: return R.id.btn_cell11;
				case 2: return R.id.btn_cell22;
				case 3: return R.id.btn_cell33;
				case 4: return R.id.btn_cell44;
				case 5: return R.id.btn_cell55;
				case 6: return R.id.btn_cell66;
				case 7: return R.id.btn_cell77;
				case 8: return R.id.btn_cell88;
				case 9: return R.id.btn_cell99;
				case 10: return R.id.btn_cell1010;
				default: return R.id.btn_cell11;
			}
		}

		int getbackupbtnid(int index)
		{
			switch (index)
			{
				case 1: return R.id.btn_cell111;
				case 2: return R.id.btn_cell222;
				case 3: return R.id.btn_cell333;
				case 4: return R.id.btn_cell444;
				case 5: return R.id.btn_cell555;
				case 6: return R.id.btn_cell666;
				case 7: return R.id.btn_cell777;
				case 8: return R.id.btn_cell888;
				case 9: return R.id.btn_cell999;
				case 10: return R.id.btn_cell101010;
				default: return R.id.btn_cell111;
			}
		}

		int getrestorebtnid(int index)
		{
			switch (index)
			{
				case 1: return R.id.btn_cell1111;
				case 2: return R.id.btn_cell2222;
				case 3: return R.id.btn_cell3333;
				case 4: return R.id.btn_cell4444;
				case 5: return R.id.btn_cell5555;
				case 6: return R.id.btn_cell6666;
				case 7: return R.id.btn_cell7777;
				case 8: return R.id.btn_cell8888;
				case 9: return R.id.btn_cell9999;
				case 10: return R.id.btn_cell10101010;
				default: return R.id.btn_cell1111;
			}
		}

		void startVM(int index)
		{
			Button btn = (Button)findViewById(getbtnid(index));
			btn.setClickable(false);
			btn.setEnabled(false);

			new Thread(new Runnable() {
				@Override
				public void run() {
					btn.setText("Starting");

					String vminitprop = "persist.sys.cell" + index + ".init";
					String vmname = "cell" + index;

					CellsPrivateServiceMgr mCellsService;
					mCellsService = new CellsPrivateServiceMgr(SystemMgrActivity.this, ServiceManager.getService("CellsPrivateService"));

					long beginTime=System.currentTimeMillis();
					mCellsService.startCellsVm(vmname);
					long ms = System.currentTimeMillis() - beginTime;
					Log.e(TAG, "start cell consumption - " + ms + "(ms).");

					int count = 30;
					while(SystemProperties.get(vminitprop,"0").equals("0") && count > 0){
						try {
							Thread.sleep(3000);
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
						count--;
					}

					if(count > 0){
						mHandler.sendMessage(Message.obtain(mHandler, MSG_START_VM_TOAST_CODE_SUCC, index));
					}else{
						mHandler.sendMessage(Message.obtain(mHandler, MSG_START_VM_TOAST_CODE_ERR, index));
					}
				}
			}).start();
		}

		void stopVM(int index)
		{
			Button btn = (Button)findViewById(getbtnid(index));
			btn.setClickable(false);
			btn.setEnabled(false);

			TextView text = (TextView)findViewById(gettextid(index));

			String vminitprop = "persist.sys.cell" + index + ".init";
			String vmname = "cell" + index;

			CellsPrivateServiceMgr mCellsService;
			mCellsService = new CellsPrivateServiceMgr(SystemMgrActivity.this, ServiceManager.getService("CellsPrivateService"));
			long beginTime=System.currentTimeMillis();
			mCellsService.stopCellsVm(vmname);
			long ms = System.currentTimeMillis() - beginTime;
			Log.e(TAG, "stop cell consumption - " + ms + "(ms).");

			if(SystemProperties.get(vminitprop).equals("0"))
			{
				btn.setText("Start");

				text.setTextColor(Color.parseColor("#000000"));
				text.setText(getResources().getString(R.string.cell) + index);

				Toast.makeText(SystemMgrActivity.this, "VPhone " +index+ " stop success!", Toast.LENGTH_SHORT).show();
			}else{
				Toast.makeText(SystemMgrActivity.this, "VPhone " +index+ " stop failure!", Toast.LENGTH_SHORT).show();
			}

			btn.setClickable(true);
			btn.setEnabled(true);
		}

		void resetVM(int index)
		{
			stopVM(index);

			String vmname = "cell" + index;

			CellsPrivateServiceMgr mCellsService;
			mCellsService = new CellsPrivateServiceMgr(SystemMgrActivity.this, ServiceManager.getService("CellsPrivateService"));
			long beginTime=System.currentTimeMillis();
			mCellsService.resetCellsVm(vmname);
			long ms = System.currentTimeMillis() - beginTime;
			Log.e(TAG, "reset cell consumption - " + ms + "(ms).");

			Toast.makeText(SystemMgrActivity.this, "VPhone " +index+ " reset success!", Toast.LENGTH_SHORT).show();
		}

		void resetVMAlert(int index)
		{
			new AlertDialog.Builder(SystemMgrActivity.this)
				.setTitle(getResources().getString(R.string.alert))
				.setMessage(getResources().getString(R.string.alertmsg))
				.setPositiveButton(getResources().getString(R.string.alertconfirm), new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int which) {
						resetVM(index);
					}
				})
				.setNegativeButton(getResources().getString(R.string.alertcancel), new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int which) {

					}
				})
			.show();
		}

		void backupVM(int index)
		{
			Button btn = (Button)findViewById(getbackupbtnid(index));
			btn.setClickable(false);
			btn.setEnabled(false);

			new Thread(new Runnable() {
				@Override
				public void run() {
					String vmtarprop = "persist.sys.cell" + index + ".tar";
					String vmname = "cell" + index;

					CellsPrivateServiceMgr mCellsService;
					mCellsService = new CellsPrivateServiceMgr(SystemMgrActivity.this, ServiceManager.getService("CellsPrivateService"));

					long beginTime=System.currentTimeMillis();
					mCellsService.tarCellsVm(vmname);
					long ms = System.currentTimeMillis() - beginTime;
					Log.e(TAG, "tar cell consumption - " + ms + "(ms).");

					int count = 300;
					while(SystemProperties.get(vmtarprop,"1").equals("1") && count > 0){
						try {
							Thread.sleep(3000);
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
						count--;
					}

					if(count > 0){
						mHandler.sendMessage(Message.obtain(mHandler, MSG_BACKUP_VM_TOAST_CODE_SUCC, index));
					}else{
						mHandler.sendMessage(Message.obtain(mHandler, MSG_BACKUP_VM_TOAST_CODE_ERR, index));
					}
				}
			}).start();
		}

		void restoreVM(int index)
		{
			stopVM(index);

			Button btn0 = (Button)findViewById(getbtnid(index));
			btn0.setClickable(false);
			btn0.setEnabled(false);

			Button btn = (Button)findViewById(getrestorebtnid(index));
			btn.setClickable(false);
			btn.setEnabled(false);

			new Thread(new Runnable() {
				@Override
				public void run() {
					String vmuntarprop = "persist.sys.cell" + index + ".untar";
					String vmname = "cell" + index;

					CellsPrivateServiceMgr mCellsService;
					mCellsService = new CellsPrivateServiceMgr(SystemMgrActivity.this, ServiceManager.getService("CellsPrivateService"));

					long beginTime=System.currentTimeMillis();
					mCellsService.untarCellsVm(vmname);
					long ms = System.currentTimeMillis() - beginTime;
					Log.e(TAG, "untar cell consumption - " + ms + "(ms).");

					int count = 300;
					while(SystemProperties.get(vmuntarprop,"1").equals("1") && count > 0){
						try {
							Thread.sleep(3000);
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
						count--;
					}

					if(count > 0){
						mHandler.sendMessage(Message.obtain(mHandler, MSG_RESTORE_VM_TOAST_CODE_SUCC, index));
					}else{
						mHandler.sendMessage(Message.obtain(mHandler, MSG_RESTORE_VM_TOAST_CODE_ERR, index));
					}
				}
			}).start();
		}

		@Override
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			switch (msg.what) {
				case MSG_START_STOP_VM:
				{
					int index = (int)msg.obj;

					String vminitprop = "persist.sys.cell" + index + ".init";
					if(SystemProperties.get(vminitprop).equals("1")){
						stopVM(index);
					}else{
						startVM(index);
					}
					break;
				}
				case MSG_START_VM_TOAST_CODE_SUCC:
				{
					int index = (int)msg.obj;

					String ip = "";
					CellsPrivateServiceMgr mCellsOtherService = new CellsPrivateServiceMgr(SystemMgrActivity.this, 
							ServiceManager.getOtherSystemService("CellsPrivateService",index));
					if(mCellsOtherService != null){
						ip = mCellsOtherService.getVmIp("");
					}

					TextView text = (TextView)findViewById(gettextid(index));
					text.setTextColor(Color.parseColor("#ff6666"));
					if(ip.length() > 0){
						getHostIP();
						// text.setText(getResources().getString(R.string.cell) + " " + index + "   IP: " + ip);
						int port = 5555 + index;
						text.setText(getResources().getString(R.string.cell) + index + " ADB IP: " + hostip + ":" + port);
					}else{
						text.setText(getResources().getString(R.string.cell) + index);
					}

					Button btn = (Button)findViewById(getbtnid(index));
					btn.setText("Stop");
					btn.setClickable(true);
					btn.setEnabled(true);

					String str = "VPhone " +index+ " start success!";
					Toast.makeText(SystemMgrActivity.this, str, Toast.LENGTH_SHORT).show();
					break;
				}
				case MSG_START_VM_TOAST_CODE_ERR:
				{
					int index = (int)msg.obj;

					Button btn = (Button)findViewById(getbtnid(index));
					btn.setClickable(true);
					btn.setEnabled(true);

					String str = "VPhone " +index+ " start failure!";
					Toast.makeText(SystemMgrActivity.this, str, Toast.LENGTH_SHORT).show();
					break;
				}
				case MSG_RESET_VM:
				{
					int index = (int)msg.obj;
					resetVMAlert(index);
					break;
				}
				case MSG_BACKUP_VM:
				{
					int index = (int)msg.obj;
					backupVM(index);
					break;
				}
				case MSG_BACKUP_VM_TOAST_CODE_SUCC:
				{
					int index = (int)msg.obj;

					Button btn = (Button)findViewById(getbackupbtnid(index));
					btn.setClickable(true);
					btn.setEnabled(true);

					Button btn1 = (Button)findViewById(getrestorebtnid(index));
					btn1.setTextColor(Color.parseColor("#00ff00"));

					String str = "VPhone " +index+ " backup success!";
					Toast.makeText(SystemMgrActivity.this, str, Toast.LENGTH_SHORT).show();
					break;
				}
				case MSG_BACKUP_VM_TOAST_CODE_ERR:
				{
					int index = (int)msg.obj;

					Button btn = (Button)findViewById(getbackupbtnid(index));
					btn.setClickable(true);
					btn.setEnabled(true);

					String str = "VPhone " +index+ " backup failure!";
					Toast.makeText(SystemMgrActivity.this, str, Toast.LENGTH_SHORT).show();
					break;
				}
				case MSG_RESTORE_VM:
				{
					int index = (int)msg.obj;
					restoreVM(index);
					break;
				}
				case MSG_RESTORE_VM_TOAST_CODE_SUCC:
				{
					int index = (int)msg.obj;

					Button btn = (Button)findViewById(getrestorebtnid(index));
					btn.setClickable(true);
					btn.setEnabled(true);
					btn.setTextColor(Color.parseColor("#000000"));

					Button btn0 = (Button)findViewById(getbtnid(index));
					btn0.setClickable(true);
					btn0.setEnabled(true);

					String str = "VPhone " +index+ " restore success!";
					Toast.makeText(SystemMgrActivity.this, str, Toast.LENGTH_SHORT).show();
					break;
				}
				case MSG_RESTORE_VM_TOAST_CODE_ERR:
				{
					int index = (int)msg.obj;

					Button btn = (Button)findViewById(getrestorebtnid(index));
					btn.setClickable(true);
					btn.setEnabled(true);

					Button btn0 = (Button)findViewById(getbtnid(index));
					btn0.setClickable(true);
					btn0.setEnabled(true);

					String str = "VPhone " +index+ " restore failure!";
					Toast.makeText(SystemMgrActivity.this, str, Toast.LENGTH_SHORT).show();
					break;
				}
				case MSG_GET_VMLIST:
				{
					for(int index=1; index <= 10; index++)
					{
						String vminitprop = "persist.sys.cell" + index + ".init";
						if (SystemProperties.get(vminitprop).equals("1"))
						{
							Button btn = (Button)findViewById(getbtnid(index));
							btn.setText("Stop");

							TextView text = (TextView)findViewById(gettextid(index));
							text.setTextColor(Color.parseColor("#ff6666"));

							String ip = "";
							CellsPrivateServiceMgr mCellsOtherService = new CellsPrivateServiceMgr(SystemMgrActivity.this, 
									ServiceManager.getOtherSystemService("CellsPrivateService",index));
							if(mCellsOtherService != null){
								ip = mCellsOtherService.getVmIp("");
							}

							if(ip.length() > 0){
								// text.setText(getResources().getString(R.string.cell) + " " + index + "   IP: " + ip);
								int port = 5555 + index;
								text.setText(getResources().getString(R.string.cell) + index + " ADB IP: " + hostip + ":" + port);
							}else{
								text.setText(getResources().getString(R.string.cell) + index);
							}
						}else{
							Button btn = (Button)findViewById(getbtnid(index));
							btn.setText("Start");

							TextView text = (TextView)findViewById(gettextid(index));

							text.setTextColor(Color.parseColor("#000000"));
							text.setText(getResources().getString(R.string.cell) + index);
						}

						String vmtarprop = "persist.sys.cell" + index + ".tar";
						if (SystemProperties.get(vmtarprop).equals("1")){
							backupVM(index);
						}

						String vmuntarprop = "persist.sys.cell" + index + ".untar";
						if (SystemProperties.get(vmuntarprop).equals("1")){
							restoreVM(index);
						}
					}
					break;
				}
			}
		}
	};

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		if(!SystemProperties.get("ro.boot.vm","1").equals("0")){
			finish();
		}else{
			setContentView(R.layout.activity_main);
		}

		Log.e(TAG, "isTabletDevice - " + isTabletDevice(SystemMgrActivity.this));
	}

	private void getHostIP()
	{
		if (hostip.length() == 0) {
			CellsPrivateServiceMgr cellsService = new CellsPrivateServiceMgr(SystemMgrActivity.this, 
					ServiceManager.getService("CellsPrivateService"));
			if(cellsService != null){
				hostip = cellsService.getVmIp("eth0");
				if (hostip.length() == 0){
					hostip = cellsService.getVmIp("wlan0");
				}
			}
		}
	}

	private boolean isTabletDevice(Context context) {
		return (context.getResources().getConfiguration().screenLayout & Configuration.SCREENLAYOUT_SIZE_MASK) >=
				Configuration.SCREENLAYOUT_SIZE_LARGE;
	}

	@Override
	public void onResume() {
		super.onResume();

		getHostIP();

		mHandler.sendMessage(Message.obtain(mHandler, MSG_GET_VMLIST));
	}

	public void btncell1(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_START_STOP_VM, 1));
	}

	public void btncell2(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_START_STOP_VM, 2));
	}

	public void btncell3(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_START_STOP_VM, 3));
	}

	public void btncell4(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_START_STOP_VM, 4));
	}

	public void btncell5(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_START_STOP_VM, 5));
	}

	public void btncell6(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_START_STOP_VM, 6));
	}

	public void btncell7(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_START_STOP_VM, 7));
	}

	public void btncell8(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_START_STOP_VM, 8));
	}

	public void btncell9(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_START_STOP_VM, 9));
	}

	public void btncell10(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_START_STOP_VM, 10));
	}


	public void btncell11(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_RESET_VM, 1));
	}

	public void btncell22(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_RESET_VM, 2));
	}

	public void btncell33(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_RESET_VM, 3));
	}

	public void btncell44(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_RESET_VM, 4));
	}

	public void btncell55(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_RESET_VM, 5));
	}

	public void btncell66(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_RESET_VM, 6));
	}

	public void btncell77(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_RESET_VM, 7));
	}

	public void btncell88(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_RESET_VM, 8));
	}

	public void btncell99(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_RESET_VM, 9));
	}

	public void btncell1010(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_RESET_VM, 10));
	}


	public void btncell111(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_BACKUP_VM, 1));
	}

	public void btncell222(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_BACKUP_VM, 2));
	}

	public void btncell333(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_BACKUP_VM, 3));
	}

	public void btncell444(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_BACKUP_VM, 4));
	}

	public void btncell555(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_BACKUP_VM, 5));
	}

	public void btncell666(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_BACKUP_VM, 6));
	}

	public void btncell777(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_BACKUP_VM, 7));
	}

	public void btncell888(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_BACKUP_VM, 8));
	}

	public void btncell999(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_BACKUP_VM, 9));
	}

	public void btncell101010(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_BACKUP_VM, 10));
	}


	public void btncell1111(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_RESTORE_VM, 1));
	}

	public void btncell2222(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_RESTORE_VM, 2));
	}

	public void btncell3333(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_RESTORE_VM, 3));
	}

	public void btncell4444(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_RESTORE_VM, 4));
	}

	public void btncell5555(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_RESTORE_VM, 5));
	}

	public void btncell6666(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_RESTORE_VM, 6));
	}

	public void btncell7777(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_RESTORE_VM, 7));
	}

	public void btncell8888(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_RESTORE_VM, 8));
	}

	public void btncell9999(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_RESTORE_VM, 9));
	}

	public void btncell10101010(View v)
	{
		mHandler.sendMessage(Message.obtain(mHandler, MSG_RESTORE_VM, 10));
	}
}
