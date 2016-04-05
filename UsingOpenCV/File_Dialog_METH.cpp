#include "stdafx.h"

//ATTENTION: SWITCH TO  MULTIBYTE (instead of UNICODE) IN PROJECT PROPERTIES

//################# COMMON FILE DIALOG CLASS METHODS ##########################
FileDialog::FileDialog(HWND parent_handle)
{
	my_hwnd = parent_handle;
	//char szFileName[MAX_PATH] ="";
	strcpy_s(szFileName,"");
	strcpy_s(szImgFolderName, "");
	//strcpy(Path_only,"");
	ZeroMemory(&my_ofn,sizeof(my_ofn));

	my_ofn.lStructSize = sizeof(my_ofn);
	my_ofn.hwndOwner = parent_handle;
	my_ofn.lpstrFilter = "All Files (*.*)\0*.*";
	my_ofn.lpstrFile = szFileName;
	my_ofn.nMaxFile = MAX_PATH;
	my_ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	//my_ofn.lpstrFileTitle = NULL;
	//my_ofn.lpstrDefExt ="txt";
	//p_szFileName = szFileName;
	//strcpy(p_szFileName,szFileName);
	p_szFileName = szFileName;
	p_szImgFolderName = szImgFolderName;

	h_Find =INVALID_HANDLE_VALUE;
	dummy_str ="";
}

static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	// If the BFFM_INITIALIZED message is received
	// set the path to the start path.
	switch (uMsg)
	{
	case BFFM_INITIALIZED:
	{
		if (NULL != lpData)
		{
			SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
		}
	}
	}

	return 0; // The function should always return 0.
}

bool FileDialog::OpenDirectory(){
	BROWSEINFO   bi;
	ZeroMemory(&bi, sizeof(bi));
	char   szDisplayName[MAX_PATH];
	strcpy_s(szDisplayName, "");

	char lpBuffer[250];
	DWORD x = GetCurrentDirectory(250, lpBuffer);

	bi.hwndOwner = my_hwnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szDisplayName;
	bi.lpszTitle = _T("Please select the folder storing the result images :");
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = BrowseCallbackProc;
	bi.lParam = (LPARAM)lpBuffer;
	bi.iImage = 0;

	strcpy_s(szImgFolderName, "");
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (NULL != pidl)
	{
		BOOL bRet = SHGetPathFromIDList(pidl, szImgFolderName);
		if (FALSE == bRet){
			MessageBox(my_hwnd, "Didn't work out right", "SORRY", MB_OK);
			return false;
		}
	}

	return true;
}

bool FileDialog::OpenFile()
{
	my_ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	bool tof = GetOpenFileName(&my_ofn);
	if (tof == false)
	{
		MessageBox(my_hwnd, "Didn't work out right", "SORRY", MB_OK);
		return false;
	}
	return true;
}

bool FileDialog::SaveFile()
{
  my_ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
  if(!GetSaveFileName(&my_ofn))
  {
     MessageBox(my_hwnd,"Didn't work","SORRY",MB_OK);
	 return false;
  }
  return true;
}

bool FileDialog::OpenFileEx()
{
   
  if(!GetOpenFileName(&my_ofn))
  {
     MessageBox(my_hwnd,"Didn't work","SORRY",MB_OK);
	 return false;
  }
   int overall_len = strlen(my_ofn.lpstrFile);
   
   char* str_lit = strrchr(my_ofn.lpstrFile,'\\');
   int path_len = overall_len - strlen(str_lit);
   this->file_name_only = str_lit;

   strncpy_s(Path_only, my_ofn.lpstrFile, path_len + 1);
   //strncpy((char*)Path_only,my_ofn.lpstrFile, path_len +1 );
   //Null string has  to be added, if strings are of different length
	
   Path_only[path_len +1] = '\0';
   path_name = Path_only; 
   strcat_s(Path_only,"*.jpg");
   //_mbscat(Path_only,"*.txt");

  return true;

 
}
char* FileDialog::Get_Name()
{
  return p_szFileName;
	// return szFileName;
}

char* FileDialog::Get_ImgFolder_Name()
{
	return p_szImgFolderName;
	// return szFileName;
}

void FileDialog::GetFilesInDirectory(std::vector<string> &out, const string &directory, const string extension)
{
	HANDLE dir;
	WIN32_FIND_DATA file_data;

	if ((dir = FindFirstFile((directory + "/*" + extension).c_str(), &file_data)) == INVALID_HANDLE_VALUE)
		return; /* No files found */

	do {
		const string file_name = file_data.cFileName;
		const string full_file_name = directory + "/" + file_name;
		const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (file_name[0] == '.')
			continue;

		if (is_directory)
			continue;

		out.push_back(file_name);
	} while (FindNextFile(dir, &file_data));

	FindClose(dir);
}

void FileDialog::PrintPathName(int posX, int posY)
{
    HDC hDC = GetDC(my_hwnd);
	int length = strlen(this->Get_Name());
	TextOut(hDC,posX,posY,this->Get_Name(),length);
	ReleaseDC(my_hwnd,hDC);
	UpdateWindow(my_hwnd);
}

void FileDialog::Store_Folders_FilesEx()
{
 
   // finds first file of folder
   h_Find = FindFirstFile(this->Path_only, &ffd);
   string cut_off; //to store string after underscore position
   int counter = 0; // if string has not the form (string_number e.g. Joseph_2)

   if (INVALID_HANDLE_VALUE == h_Find) 
   {
      MessageBox(my_hwnd,"Files not available","Sorry",MB_OK);
      return;
   }   

    do
   {  
	   // returns next file_name
       dummy_str =  ffd.cFileName;
	   // looks for last position of underscore and returns index in string
	   int where_ = dummy_str.find_last_of("_");
	   if (where_ != -1)
	   {
	   // returns string after "_" e.g. Ludwig_16 -> 16
	   cut_off = dummy_str.substr(where_ +1 ,dummy_str.length() - where_);
	   // turns string after "_" into integer
	   int index = atoi(cut_off.c_str());
	   // add string to map (string as value; and number after "_" as key)
       file_map[index] = dummy_str;
	   }
	   else
	   {
		   counter++;
         file_map[counter] = dummy_str;
       }
      
   }
   while (FindNextFile(h_Find, &ffd) != 0); //finds next file in folder
   
   
   this->it = file_map.begin();

}

string FileDialog::Get_Folder_FilenameEx()
{

  // return this->file_list[index];
    if(it == file_map.end())
	{  
      it = file_map.begin();
	  return "FIN";
	}
	string str = it->second;
	it++;
	return str;
	
   
}

string FileDialog::Get_PathNameEx()
{
   
   return this->path_name;
}

string FileDialog::Get_FilenameEx()
{
	return this->file_name_only;
  
}

int FileDialog::Get_Map_Size()
{

  return this->file_map.size();
}

//END COMMON FILE DIALOG