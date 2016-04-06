
// IMPORTANT: HOW TO IMPLEMENT AND MAKE USE OF OPENCV CODE

// ATTENTION: despite of being oldfashioned PROGRAM USES MULTIBYTE INSTEAD OF UNICODE
// find a illustrative tutorial about UNICODE and Multibyte (ansi) at
// http://www.codeproject.com/Articles/76252/What-are-TCHAR-WCHAR-LPSTR-LPWSTR-LPCTSTR-etc

// ATTENTION: Program requires the right codecs to be installed (download a codec package from the internet)

// ATTENTION:  ffdshow might also be needed to run the program (there are free downloads)

/* I used Visual C++ Express Edition 2010 on a Windows 7 machine (if you want to create a x64 application you need to download SDK 7.1),
   but the following explanations, I think, might also apply to older or other versions of Visual C++.
   I created an x86 application and did not try out the x64 version, therefore I can not guarantee that the instructions for x64 will lead to a useable application  

 1)
 Download and install
 add new environmental variables (Umgebungsvariablen); enter path to access opencv dll files, which are needed to process code
       => Press 'New' in Window 'Environmental Variables' 
	      Enter 'Name' (e.g. OPENCV_PATH)
		  and under 'Value' enter the path to the opencv folder containing the nescessary opencv dll's .
		     I used OpenCV 2.4.3 and the x86 platform 
			 that's why I entered ' C:\opencv\build\x86\vc10\bin ' (depends on the folder you have chosen during installation)
			 If you want to use the x64 version
			 it could be ' C:\opencv\build\x64\vc10\bin ' instead 

       => if you prefer not to add a new path to the environmental folder or if it does not work:
	         Then copy all dll's in the above mentioned folder (depending on what you have chosen: x86 or x64)
			 into  the folder where your .exe file is located.
			 You might also need the 'tbb.dll' files which are located somewhere in the opencv folders (just search for tbb.dll)

3)
Add libraries and header files

       => After you have chosen Win32 (or Win32 console). The opencv libraries and headers have to be included to be available.
	   
	   => Select 'Project/Properties'
	         Open 'C/C++' and click on 'General' (Allgemein) and enter path of opencv include directories into the field 'additional dependencies' (zusätzliche Abhängigkeiten)
			 depending on the installation folder this could be 'C:\opencv\build\include' (for an x86 application like here)
			 
			 For a x64 application first open the 'configuration manager' and choose new platform 'x64' (you need SDK 7.1 for that).
			 Then do the same as described above and add the path to the nescessary opencv header files

       => Select 'Project/Properties'
	          Open 'Linker', select 'General' and enter path into opencv libraries using the field 'additional libraries' ,
			  depending on the installation folder this could be 'C:\opencv\build\x86\vc10\lib' for an x86 application like this one here
			  or 'C:\opencv\build\x64\vc10\lib' for an x64 application 

       => Select 'Project/Properties'
	          Open 'Linker', select 'Input' (Eingabe) and add opencv lib. files to the field 'additional dependencies' .
			  e.g. (files with a d at the end of the file's names are the debug versions)
			  This could be 'opencv_core243.lib;opencv_highgui243.lib;opencv_imgproc243.lib;opencv_features2d243.lib;opencv_flann243.lib;%(AdditionalDependencies) '
			  etc. for an x86 application
			  and for x64 appliaction the correpsonding ones from x64 folder described above in Project/Properties/Linker/General
			  

4)
Include opencv header files (here in stdafx.h file)
 
 */

#include "stdafx.h"
#include "UsingOpenCV.h"
#define MAX_LOADSTRING 100
#define MAX_FRAME_SEQ 1296000 //15fps*3600 second *24 hour
#define UNKNOW 0
#define PARK 1
#define LEAVE 2

#define SHOW_LOG_SIZE 5

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// titlestring
TCHAR szWindowClass[MAX_LOADSTRING];			//classname of main window

// declaration of Functions of this code module
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

// Implemented as modeless dialog; runs parallel to main application // see [REF]
//BOOL CALLBACK   Loaded_Files_Proc(HWND, UINT, WPARAM, LPARAM);

//GLOBAL USERDEFINED VARIABLES

 HWND hWnd; //handle of Main Window

 //handles for controls // placed on main window; see WM_CREATE
 HWND img_path_caption_handle;
 HWND img_pathinfo_handle; 
 HWND video_path_caption_handle;
 HWND video_pathinfo_handle;

 HWND videoName_handle;
 HWND trackId_handle;
 HWND from_caption_handle; 
 HWND to_caption_handle;
 HWND from_handle; 
 HWND to_handle;

 HWND framediff_caption_handle;
 HWND distance_caption_handle;
 HWND framediff_handle;
 HWND distance_handle;

 HWND button_handle; 
 HWND leftnavi_button_handle;
 HWND rightnavi_button_handle;
 HMENU menu_handle;		 
 HWND track_bar_handle;
 HWND trackbar_info_handle;
 HWND check_box_handles[4];

 HWND save_button_handle;
 HWND merge_checkbox_handle;
 HWND spot_handle;

 HWND result_handle;

// for File Dialog Operations
FileDialog * fd; //Pointer to FileDialog Class, will be intialized in WM_CREATE
char PathName[MAX_PATH]; //char stores pathname of FileDialog

// Variables for Video operations (see also Image_OP and Video_OP in OpenCv_Video_OP DEF and METH)
Video_OP * vio;
char* movie_full_path;
char* save_file_movie;
string movie_file_name;
//Current folder
char lpBuffer[250];

//Variables for Image matching
ImageInfoSet* imginfoset;
string imagefolderPath = "";
string videofolderPath = "";
string currentBaseWindowName;
string currentMatchWindowName;
ImageInfo* curBaseImageInfo;
ImageInfo* curMatchImageInfo;
bool hasPotentialMatch = false;

//Variables for saving
string outputPath = "";
ofstream outputStream;
deque<string> outputLog;

//warning contollers
bool showClearTutorial = true;
bool enableClear = false;
bool showVideoConfigWarning = true;

//GLOBAL USERDEFINED FUNCTIONS
int Get_Number(HWND handle);
void Transfer_Num_as_Text(HWND handle, int number);
void Set_Text(HWND handle, char* str);
void Display_String(HWND handle,int where_x, int where_y, char* str);
//bool MoveIndex(int &index, int movement, int low, int high);
void LoadTypedImage(int type, ImageInfo* imgInfo = NULL);
void HandleMovement(int movement);
void DestroyImage(int type);

//save functions
bool saveMerge(int baseTrack, int matchTrack);
bool saveSpotConfig(int type, int trackID, int spotNum);
bool appendTxtToOutput(string content);
bool closeOutputFile();

//Functions for UI enable/disable
void EnableVideoInteractions(bool enable);
void EnableBaseImage(bool enable);
void EnableBaseImageInteractions(bool enable);
void EnableMatchImage(bool enable);
void EnableMatchImageInteractions(bool enable);
void EnableNavigator(bool enable);
void EnableAllInteractions(bool enable);
void ClearBaseImageInteractions();
void ClearMatchImageInteractions();


int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	// TODO //Put in code here
	MSG msg;
	HACCEL hAccelTable;

	//Intialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_USINGOPENCV, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// intialize application
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_USINGOPENCV);

	//main message loop
	while (GetMessage(&msg, NULL, 0, 0))
	{

		// if(!IsDialogMessage(dialog_handle,&msg))//EXTRA LINE OF CODE FOR MODELESS DIALOG MESSAGES // see [REF]
		//{//IF 1
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//}//END IF 1
	}

	return (int)msg.wParam;
}



//
//  FUNKTION: MyRegisterClass()
//
//  Purpose: Registriert die Fensterklasse. 
//

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_USINGOPENCV);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = (LPCTSTR)IDC_USINGOPENCV;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   Purpose: saves handle of instance and creates main window
//
//

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // handle of instance

	//hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	//  CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL); //OR
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		CW_USEDEFAULT, 0, 550, 520, NULL, NULL, hInstance, NULL);


	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigniert, WORD, LONG)
//
// PUPOSE:  handling of messages from main window  
//
//  WM_COMMAND	- Processes message coming from the menu and the controls (e.g. button)
//  WM_PAINT	- Re/Drawing of main window
//  WM_DESTROY	- Ends application
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// handling commands from menu and controls (e.g. buttons)
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;

		case IDC_CHECK_BOX_BASE:
			if (videofolderPath.compare("") == 0 && showVideoConfigWarning){
				MessageBox(hWnd, "You haven't configure the video directory, which may lead to an error when trying to play video.", "WARNNING", MB_OK);
				showVideoConfigWarning = false;
			}

			DestroyImage(MATCH_TYPE);
			HandleMovement(0);
			//need to first disable, then enable, otherwise same button will be disabled by mistake
			EnableMatchImageInteractions(false);
			EnableVideoInteractions(true);
			EnableNavigator(true);
			EnableBaseImageInteractions(true);

			break;

		case IDC_CHECK_BOX_MATCH:
			HandleMovement(0);
			//need to first disable, then enable, otherwise same button will be disabled by mistake
			EnableBaseImageInteractions(false);
			EnableMatchImageInteractions(true);
			EnableNavigator(true);
			break;

		case IDC_BUTTON_LEFT:
			HandleMovement(-1);
			break;

		case IDC_BUTTON_RIGHT:
			HandleMovement(+1);
			break;

		case IDC_BUTTON_SAVE:
		{
			if (BST_CHECKED == SendMessage(check_box_handles[0], BM_GETCHECK, 0, 0)){
				int trackID = Get_Number(trackId_handle);
				int type = UNKNOW;
				if (BST_CHECKED == SendMessage(check_box_handles[2], BM_GETCHECK, 0, 0)){
					type = PARK;
				}
				else if (BST_CHECKED == SendMessage(check_box_handles[3], BM_GETCHECK, 0, 0)){
					type = LEAVE;
				}

				int spotNum = Get_Number(spot_handle);
				if (spotNum < 0 || type == UNKNOW){
					MessageBox(hWnd, "Either SpotNum or Park/Leave type is not set yet, please check and save again!", "WARNING", MB_OK);
				}
				else{
					if (saveSpotConfig(type, trackID, spotNum)){
						Set_Text(spot_handle, "");
						PostMessage(type == PARK ? check_box_handles[2] : check_box_handles[3], BM_SETCHECK, BST_UNCHECKED, 0);
					}
				}
			}
			else if (BST_CHECKED == SendMessage(check_box_handles[1], BM_GETCHECK, 0, 0)){
				if (BST_CHECKED == SendMessage(merge_checkbox_handle, BM_GETCHECK, 0, 0)){
					if (saveMerge(curBaseImageInfo->trackNum, curMatchImageInfo->trackNum)){
						PostMessage(merge_checkbox_handle, BM_SETCHECK, BST_UNCHECKED, 0);
					}
				}
				else{
					MessageBox(hWnd, "The merge radio button is not set yet, please check and save again!", "WARNING", MB_OK);
				}
			}
			else{
				MessageBox(hWnd, "This should never be shown!", "ERROR", MB_OK);
			}
		}
			break;

		case IDC_BUTTON_DO_IT:
		{
			char buf[15];
			int x = GetWindowText(button_handle, buf, 15);
			if (strcmp(buf, "PLAY") == 0){
				char videoName[MAX_PATH];
				x = GetWindowText(videoName_handle, videoName, MAX_PATH);
				string fullVideoPath = videofolderPath + "\\" + string(videoName);
				if (vio->Get_Video_from_File((char*)fullVideoPath.c_str())){
					Set_Text(button_handle, "STOP");

					int start = max(Get_Number(from_handle), 0);
					int end = max(Get_Number(to_handle), 0);
					start = start > end ? 0 : start;
					vio->Play_Video(start, end);

					Set_Text(button_handle, "PLAY");
				}
				else{
					MessageBox(hWnd, "Failed to open the video file, please verify your video dir setup!", "SORRY", MB_OK);
				}
			}
			else
			{
				vio->Stop_Video();
				Set_Text(button_handle, "PLAY");
			}
		}
		break;

		case IDM_IMAGEOP_LOADIMAGEFOLDER:
		{
			if (fd->OpenDirectory()){
				imagefolderPath = fd->Get_ImgFolder_Name();
				std::vector<string> imageNameList;
				fd->GetFilesInDirectory(imageNameList, imagefolderPath, ".jpg");
				Set_Text(img_pathinfo_handle, (char*)imagefolderPath.c_str());
				imginfoset = new ImageInfoSet(imageNameList);
				EnableBaseImage(true);
			}
		}
		break;

		case IDM_IMAGEOP_SELECTVIDEOFOLDER:
		{
			if (fd->OpenDirectory()){
				videofolderPath = fd->Get_ImgFolder_Name();
				//fd->GetFilesInDirectory(imageNameList, imagefolderPath, ".jpg");
				Set_Text(video_pathinfo_handle, (char*)videofolderPath.c_str());
			}
		}
		break;

		case IDM_IMAGEOP_SELECTOUTPUTFILE:
		{
			if (fd->OpenFile()){
				closeOutputFile();
				outputPath = fd->Get_Name();
				outputStream.open(outputPath, ios::out | ios::app);

				time_t now = time(0); 
				char buf[100];
				ctime_s(buf, 100, &now);
				appendTxtToOutput("OpenFile : " + outputPath + " @ " + string(buf));
			}
			else{
				MessageBox(hWnd, "Failed to open the output file!", "SORRY", MB_OK);
			}
			
		}
		break;

		case IDM_EXIT:
			vio->Stop_Video();
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
		break;

	case WM_CREATE:
	{
		fd = new FileDialog(hWnd);
		vio = new Video_OP();

		// Static control as caption for path info textfield
		img_path_caption_handle = CreateWindowEx(WS_EX_TRANSPARENT, "STATIC", "ImageDir:", WS_CHILD | WS_VISIBLE,
			10, 10, 70, 30, hWnd, (HMENU)IDC_STATIC_IMAGE_PATH, GetModuleHandle(NULL), NULL);
		//SetBkColor(img_path_caption_handle, GetSysColor(COLOR_WINDOW));

		// Textfield for image pathinfo
		img_pathinfo_handle = CreateWindowEx(WS_EX_STATICEDGE, "STATIC", "", WS_CHILD | WS_VISIBLE,
			85, 10, 440, 30, hWnd, (HMENU)IDC_EDIT_IMAGE_PATH, GetModuleHandle(NULL), NULL);

		video_path_caption_handle = CreateWindowEx(WS_EX_TRANSPARENT, "STATIC", "VideoDir:", WS_CHILD | WS_VISIBLE,
			10, 50, 70, 30, hWnd, (HMENU)IDC_STATIC_VIDEO_PATH, GetModuleHandle(NULL), NULL);
		// Textfield for video pathinfo
		video_pathinfo_handle = CreateWindowEx(WS_EX_STATICEDGE, "STATIC", "", WS_CHILD | WS_VISIBLE,
			85, 50, 440, 30, hWnd, (HMENU)IDC_EDIT_VIDEO_PATH, GetModuleHandle(NULL), NULL);

		//videoName
		CreateWindowEx(WS_EX_TRANSPARENT, "STATIC", "CurVideo:", WS_CHILD | WS_VISIBLE,
			10, 90, 70, 30, hWnd, (HMENU)IDC_STATIC_VIDENAME, GetModuleHandle(NULL), NULL);
		videoName_handle = CreateWindowEx(WS_EX_STATICEDGE, "STATIC", "", WS_CHILD | WS_VISIBLE,
			85, 90, 300, 30, hWnd, (HMENU)IDC_EDIT_VIDEONAME, GetModuleHandle(NULL), NULL);

		//trackId
		CreateWindowEx(WS_EX_TRANSPARENT, "STATIC", "CurTrack:", WS_CHILD | WS_VISIBLE,
			400, 90, 70, 30, hWnd, (HMENU)IDC_STATIC_TRACK, GetModuleHandle(NULL), NULL);
		trackId_handle = CreateWindowEx(WS_EX_STATICEDGE, "STATIC", "", WS_CHILD | WS_VISIBLE,
			475, 90, 50, 30, hWnd, (HMENU)IDC_EDIT_TRACK, GetModuleHandle(NULL), NULL);

		int horzSeparatorPos = 130;
		CreateWindowEx(WS_EX_CLIENTEDGE, "Static", "", SS_ETCHEDHORZ | WS_CHILD | WS_VISIBLE, 5, horzSeparatorPos, 535, 10, hWnd, NULL, GetModuleHandle(NULL), NULL);

		// Static control saying FROM
		CreateWindowEx(WS_EX_TRANSPARENT, "STATIC", "Video Info Section", WS_CHILD | WS_VISIBLE,
			10, horzSeparatorPos + 10, 145, 30, hWnd, NULL, GetModuleHandle(NULL), NULL);

		from_caption_handle = CreateWindowEx(WS_EX_TRANSPARENT, "STATIC", "Frame start:", WS_CHILD | WS_VISIBLE,
			10, horzSeparatorPos + 50, 90, 30, hWnd, (HMENU)IDC_STATIC_START, GetModuleHandle(NULL), NULL);
		// Textfield to fill in frame where video shall start from
		from_handle = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE,
			105, horzSeparatorPos + 50, 50, 30, hWnd, (HMENU)IDC_EDIT_START, GetModuleHandle(NULL), NULL);

		// Static control saying TO
		to_caption_handle = CreateWindowEx(WS_EX_TRANSPARENT, "STATIC", "Frame end:", WS_CHILD | WS_VISIBLE,
			10, horzSeparatorPos + 90, 90, 30, hWnd, (HMENU)IDC_STATIC_STOP, GetModuleHandle(NULL), NULL);
		// Textfield for telling program where to stop 
		to_handle = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE,
			105, horzSeparatorPos + 90, 50, 30, hWnd, (HMENU)IDC_EDIT_STOP, GetModuleHandle(NULL), NULL);

		// Button to start processes which were selected in the menu
		button_handle = CreateWindowEx(WS_EX_CLIENTEDGE, "BUTTON", "PLAY", WS_CHILD | WS_VISIBLE,
			40, horzSeparatorPos + 130, 80, 40, hWnd, (HMENU)IDC_BUTTON_DO_IT, GetModuleHandle(NULL), NULL);

		int vertSeparatorPos = 170;
		CreateWindowEx(WS_EX_CLIENTEDGE, "Static", "", SS_ETCHEDVERT | WS_CHILD | WS_VISIBLE, vertSeparatorPos, horzSeparatorPos + 1, 100, 181, hWnd, NULL, GetModuleHandle(NULL), NULL);

		// commoncontrol DLL needs to be loaded (needed for trackbar) 
		//Attention: also requires <commctrl.h> and comctl32.lib
		INITCOMMONCONTROLSEX icc;
		icc.dwICC = ICC_BAR_CLASSES;
		icc.dwSize = sizeof(icc);
		bool z = InitCommonControlsEx(&icc);

		//frame diff and distance filters
		framediff_caption_handle = CreateWindowEx(WS_EX_TRANSPARENT, "STATIC", "Framediff Thres:", WS_CHILD | WS_VISIBLE,
			vertSeparatorPos + 10, horzSeparatorPos + 10, 110, 30, hWnd, (HMENU)IDC_STATIC_FRAME_DIFF, GetModuleHandle(NULL), NULL);
		framediff_handle = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", std::to_string(DEFAULT_FRAMEDIFF_THRES).c_str(), WS_CHILD | WS_VISIBLE,
			vertSeparatorPos + 125, horzSeparatorPos + 10, 50, 30, hWnd, (HMENU)IDC_EDIT_FRAME_DIFF, GetModuleHandle(NULL), NULL);

		int thirdcolPos = vertSeparatorPos + 180;

		distance_caption_handle = CreateWindowEx(WS_EX_TRANSPARENT, "STATIC", "Distance Thres:", WS_CHILD | WS_VISIBLE,
			thirdcolPos + 10, horzSeparatorPos + 10, 110, 30, hWnd, (HMENU)IDC_STATIC_DISTANCE_THRES, GetModuleHandle(NULL), NULL);
		distance_handle = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", std::to_string(DEFAULT_DISTANCE_THRES).c_str(), WS_CHILD | WS_VISIBLE,
			thirdcolPos + 125, horzSeparatorPos + 10, 50, 30, hWnd, (HMENU)IDC_EDIT_DISTANCE_THRES, GetModuleHandle(NULL), NULL);

		//base/match image selector
		check_box_handles[0] = CreateWindowEx(WS_EX_STATICEDGE, "BUTTON", "Base Image", WS_CHILD | WS_VISIBLE | WS_GROUP | BS_AUTORADIOBUTTON,
			vertSeparatorPos + 10, horzSeparatorPos + 50, 165, 30, hWnd, (HMENU)IDC_CHECK_BOX_BASE, GetModuleHandle(NULL), NULL);
		check_box_handles[1] = CreateWindowEx(WS_EX_STATICEDGE, "BUTTON", "Match Image", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			vertSeparatorPos + 10, horzSeparatorPos + 90, 165, 30, hWnd, (HMENU)IDC_CHECK_BOX_MATCH, GetModuleHandle(NULL), NULL);

		//merge checkbox
		merge_checkbox_handle = CreateWindowEx(WS_EX_STATICEDGE, "BUTTON", "Merge Match to Base", WS_CHILD | BS_AUTOCHECKBOX,
			thirdcolPos + 10, horzSeparatorPos + 50, 165, 30, hWnd, (HMENU)IDC_CHECK_BOX_MERGE, GetModuleHandle(NULL), NULL);

		//base/match image selector
		check_box_handles[2] = CreateWindowEx(WS_EX_STATICEDGE, "BUTTON", "Park", WS_CHILD | WS_VISIBLE | WS_GROUP | BS_AUTORADIOBUTTON,
			thirdcolPos + 10, horzSeparatorPos + 50, 80, 30, hWnd, (HMENU)IDC_CHECK_BOX_PARK, GetModuleHandle(NULL), NULL);
		check_box_handles[3] = CreateWindowEx(WS_EX_STATICEDGE, "BUTTON", "Leave", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			thirdcolPos + 95, horzSeparatorPos + 50, 80, 30, hWnd, (HMENU)IDC_CHECK_BOX_LEAVE, GetModuleHandle(NULL), NULL);

		//change spot
		CreateWindowEx(WS_EX_TRANSPARENT, "Static", "New Spot:", WS_CHILD | WS_VISIBLE,
			thirdcolPos + 10, horzSeparatorPos + 90, 110, 30, hWnd, (HMENU)IDC_STATIC_SPOT, GetModuleHandle(NULL), NULL);
		spot_handle = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE,
			thirdcolPos + 125, horzSeparatorPos + 90, 50, 30, hWnd, (HMENU)IDC_EDIT_SPOT, GetModuleHandle(NULL), NULL);

		//navigator
		leftnavi_button_handle = CreateWindowEx(WS_EX_CLIENTEDGE, "BUTTON", "<---", WS_CHILD | WS_VISIBLE,
			vertSeparatorPos + 100 - 80, horzSeparatorPos + 130, 80, 40, hWnd, (HMENU)IDC_BUTTON_LEFT, GetModuleHandle(NULL), NULL);
		rightnavi_button_handle = CreateWindowEx(WS_EX_CLIENTEDGE, "BUTTON", "--->", WS_CHILD | WS_VISIBLE,
			vertSeparatorPos + 185 - 80, horzSeparatorPos + 130, 80, 40, hWnd, (HMENU)IDC_BUTTON_RIGHT, GetModuleHandle(NULL), NULL);
		//save review result
		save_button_handle = CreateWindowEx(WS_EX_CLIENTEDGE, "BUTTON", "SAVE", WS_CHILD | WS_VISIBLE,
			vertSeparatorPos + 275, horzSeparatorPos + 130, 80, 40, hWnd, (HMENU)IDC_BUTTON_SAVE, GetModuleHandle(NULL), NULL);

		horzSeparatorPos += 175;
		CreateWindowEx(WS_EX_CLIENTEDGE, "Static", "", SS_ETCHEDHORZ | WS_CHILD | WS_VISIBLE, 5, horzSeparatorPos, 535, 10, hWnd, NULL, GetModuleHandle(NULL), NULL);
		
		CreateWindowEx(WS_EX_TRANSPARENT, "Static", "Latest Logs:", WS_CHILD | WS_VISIBLE,
			10, horzSeparatorPos + 10, 515, 30, hWnd, (HMENU)IDC_STATIC_RESULT, GetModuleHandle(NULL), NULL);
		result_handle = CreateWindowEx(WS_EX_TRANSPARENT, "STATIC", " Waiting for configurting the output file!", WS_CHILD | WS_VISIBLE,
			10, horzSeparatorPos + 40, 515, 110, hWnd, (HMENU)IDC_EDIT_RESULT, GetModuleHandle(NULL), NULL);

		EnableAllInteractions(false);

		// Gets Menu Handle
		menu_handle = GetMenu(hWnd);
		CheckMenuItem(menu_handle, IDM_PLAY_MOV, MF_CHECKED);
		
	}
	break;
	// press left mouse button somewhere in the main window to uncheck all radiobuttons (optionbuttons)
	case WM_LBUTTONDOWN:
	{
		if (BST_CHECKED == SendMessage(check_box_handles[0], BM_GETCHECK, 0, 0)){
			if (showClearTutorial){
				int ret = MessageBox(hWnd, "Enable the function to clear park/leave radio when clicking the background or a non-active component?", "You just clicked the background or a non-active component", MB_YESNO);
				showClearTutorial = false;
				enableClear = (ret == IDYES);
			}
		}
		
		if (enableClear){
			SendMessage(check_box_handles[2], BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(check_box_handles[3], BM_SETCHECK, BST_UNCHECKED, 0);
		}
	}
	break;

	case WM_KEYDOWN:
	{
		int x = 0;
		switch (wParam)
		{
		case VK_LEFT:
			HandleMovement(-1);
			break;

		case VK_RIGHT:
			HandleMovement(+1);
			break;
		}
	}
	break;

	case WM_DESTROY:
		vio->Stop_Video();
		DestroyImage(BASE_TYPE);
		DestroyImage(MATCH_TYPE);
		closeOutputFile();

		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


// Message Loop for Dialog
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

// message handler for loaded files dialog  // not implemented in compiled program (see [REF]
/*
BOOL CALLBACK Loaded_Files_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;
    case WM_COMMAND:
		if (LOWORD(wParam) == IDC_CLOSE_LOADED_FILES ) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;

	}
	return FALSE;
}
*/

//ADDITIONAL GLOBAL FUNCTIONS
void DestroyImage(int type){
	const char* toDestroy = (type == BASE_TYPE) ? currentBaseWindowName.c_str() : currentMatchWindowName.c_str();
	vio->Destroy_Image(toDestroy);
}

void LoadTypedImage(int type, ImageInfo *imginfo){
	if (imginfo == NULL){
		return;
	}

	string currentImageName = imagefolderPath + "\\" + imginfo->imageName;
	string prefix = (type == BASE_TYPE) ? "Base-" : "Match-";
	RECT rc;
	GetClientRect(hWnd, &rc);
	MapWindowPoints(hWnd, GetParent(hWnd), (LPPOINT)&rc, 2);
	int x = (type == BASE_TYPE) ? rc.right : rc.right + IMG_WIDTH + 17;
	int y = rc.top ;
	string windowName = prefix + " startFrame=" + std::to_string(imginfo->startFrame) + " endFrame=" + std::to_string(imginfo->endFrame) 
		+ " videoName=" + imginfo->videoName;
	vio->Load_Image(windowName.c_str(), currentImageName.c_str(), x + 8, y - 50, IMG_WIDTH, IMG_HEIGHT);

	if (type == BASE_TYPE && windowName.compare(currentBaseWindowName) || type == MATCH_TYPE && windowName.compare(currentMatchWindowName)){
		DestroyImage(type);
	}

	if (type == BASE_TYPE){
		currentBaseWindowName = windowName;
		curBaseImageInfo = imginfo;
	}
	else{
		currentMatchWindowName = windowName;
		curMatchImageInfo = imginfo;
	}

	if (type == BASE_TYPE){
		Set_Text(videoName_handle, (char*)(imginfo->videoName+".avi").c_str());
		Set_Text(trackId_handle, (char*)std::to_string(imginfo->trackNum).c_str());
		Set_Text(from_handle, (char*)std::to_string(imginfo->startFrame).c_str());
		Set_Text(to_handle, (char*)std::to_string(imginfo->endFrame).c_str());
	}
	else{
		if (curMatchImageInfo->videoName.compare(curBaseImageInfo->videoName) == 0){
			Set_Text(to_handle, (char*)std::to_string(max(curMatchImageInfo->endFrame, curBaseImageInfo->endFrame)).c_str());
		}
		else{
			Set_Text(to_handle, (char*)std::to_string(MAX_FRAME_SEQ).c_str());
		}
	}
}

void HandleMovement(int movement){
	int frameDiff = Get_Number(framediff_handle);
	int distance = Get_Number(distance_handle);

	if (BST_CHECKED == SendMessage(check_box_handles[0], BM_GETCHECK, 0, 0)){
		ImageInfo* imginfo = imginfoset->getNextBaseImageInfo(movement, hasPotentialMatch, 
			frameDiff == 0 ? DEFAULT_FRAMEDIFF_THRES : frameDiff, distance == 0? DEFAULT_DISTANCE_THRES: distance);
		if (imginfo == NULL){
			MessageBox(hWnd, "Reached the boundary. Do nothing!", "SORRY", MB_OK);
			return;
		}

		LoadTypedImage(BASE_TYPE, imginfo);
		EnableMatchImage(hasPotentialMatch);
	}
	else if (BST_CHECKED == SendMessage(check_box_handles[1], BM_GETCHECK, 0, 0)){
		ImageInfo* imginfo = imginfoset->getNextMatchImageInfo(movement,
			frameDiff == 0 ? DEFAULT_FRAMEDIFF_THRES : frameDiff, distance == 0 ? DEFAULT_DISTANCE_THRES : distance);
		if (imginfo == NULL){
			MessageBox(hWnd, "Reached the boundary. Do nothing!", "SORRY", MB_OK);
			return;
		}
		
		LoadTypedImage(MATCH_TYPE, imginfo);
	}

	SetFocus(hWnd);
}

// displays char String in Window of selected handle
void Display_String(HWND handle, int where_x, int where_y, char* str)
{
	HDC hDC = GetDC(handle);
	TextOut(hDC, where_x, where_y, str, strlen(str));
	ReleaseDC(handle, hDC);
}

// returns number contained in a static or edit control
int Get_Number(HWND handle){
	char buf[30];
	int x = GetWindowText(handle, buf, 30);
	if (strlen(buf) > 0)
	{
		return atoi(buf);
	}

	return -1;
}
// transfer Integer to a static or edit control
void Transfer_Num_as_Text(HWND handle, int number)
{
	char buf[30];
	sprintf_s(buf, "%d", (int)number);
	SendMessage((HWND)handle, (UINT)WM_SETTEXT, NULL, (LPARAM)buf);
}

// transfers Text (i.e. char) to control 
void Set_Text(HWND handle, char* str)
{
	SendMessage((HWND)handle, (UINT)WM_SETTEXT, NULL, (LPARAM)str);
}

void EnableVideoInteractions(bool enable){
	EnableWindow(from_handle, enable);
	EnableWindow(to_handle, enable);
	EnableWindow(button_handle, enable);
}

void EnableBaseImage(bool enable){
	EnableWindow(check_box_handles[0], enable);
}

void EnableNavigator(bool enable){
	EnableWindow(leftnavi_button_handle, enable);
	EnableWindow(rightnavi_button_handle, enable);
}

void EnableBaseImageInteractions(bool enable){
	ShowWindow(GetDlgItem(hWnd, IDC_STATIC_SPOT), enable ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hWnd, IDC_EDIT_SPOT), enable ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hWnd, IDC_CHECK_BOX_PARK), enable ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hWnd, IDC_CHECK_BOX_LEAVE), enable ? SW_SHOW : SW_HIDE);

	EnableWindow(spot_handle, enable);

	EnableWindow(check_box_handles[2], enable);
	EnableWindow(check_box_handles[3], enable);
	EnableWindow(save_button_handle, enable);
}

void EnableMatchImageInteractions(bool enable){
	ShowWindow(GetDlgItem(hWnd, IDC_CHECK_BOX_MERGE), enable ? SW_SHOW : SW_HIDE);

	EnableWindow(merge_checkbox_handle, enable);
	EnableWindow(save_button_handle, enable);
}

void EnableMatchImage(bool enable){
	EnableWindow(check_box_handles[1], enable);
}

void EnableAllInteractions(bool enable){
	EnableBaseImageInteractions(enable);
	EnableMatchImageInteractions(enable);
	EnableVideoInteractions(enable);
	EnableMatchImage(enable);
	EnableBaseImage(enable);
	EnableNavigator(enable);
}

void ClearBaseImageInteractions(){
	SendMessage(check_box_handles[2], BM_SETCHECK, BST_UNCHECKED, 0);
	SendMessage(check_box_handles[3], BM_SETCHECK, BST_UNCHECKED, 0);
	Set_Text(spot_handle, "");
}

void ClearMatchImageInteractions(){
	SendMessage(merge_checkbox_handle, BM_SETCHECK, BST_UNCHECKED, 0);
}

bool saveMerge(int baseTrack, int matchTrack){
	string content = "MERGE fromTrack= " + std::to_string(baseTrack) + " toTrack= " + std::to_string(matchTrack);
	return appendTxtToOutput(content);
}

bool saveSpotConfig(int type, int trackID, int spotNum){
	string operation = (type == PARK) ? "PARK" : "LEAVE";
	string content = operation + " track=" + std::to_string(trackID) + " spot= " + std::to_string(spotNum);
	return appendTxtToOutput(content);
}

bool appendTxtToOutput(string content){
	if (outputPath.compare("") == 0){
		MessageBox(hWnd, "Save log failed. Please set up output file and try to save again!", "WARNING", MB_OK);
		return false;
	}

	outputStream << content << endl;
	outputLog.push_back(content);
	while (outputLog.size() > SHOW_LOG_SIZE){
		outputLog.pop_front();
	}

	string log = "";
	for (int i = 0; i < outputLog.size(); i++){
		log += " " + outputLog.at(i) + "\n";
	}
	Set_Text(result_handle, (char*)log.c_str());
	return true;
}

bool closeOutputFile(){
	if (outputPath.compare("") == 0){
		return false;
	}
	outputStream.close();
	return true;
}