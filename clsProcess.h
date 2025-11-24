#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include <sstream>//not sure what this is for but i used it for somthing that works like split function
#include <iomanip>

using namespace std;

//to i know how many processes is there
static int NumberOfProcesses = 0;


class clsProcess
{
private:
	//every process have these
	string _ProcessID;
	int _ArrivalTime;
	int _ProcessTime;


	//for the output(everyprocess should have these in the output)
	int _TurnaroundTime;
	int _Delay;
	int _ResponseTime;

	//during the sjf excution (and at the end of it) the processes should have these to get the rest of the info
	int _EndTime;
	int _StartTime;

	//works like the locks
	bool _IsCpuFree = 1;


	//gets the info from the file
	static clsProcess _ConvertLineToProcessObject(string line, string sep = " ") {

		vector<string>vProcessData;

		//this is a new methoed i found on the web (like a split fuction)
		stringstream ss(line);

		string idpart, arpart, ptpart;
		//takes everypart and put it for the constrctor
		ss >> idpart;
		ss >> arpart;
		ss >> ptpart;

		return clsProcess(idpart, stof(arpart), stof(ptpart));


	}


	//convert all the info i calculated to string and put it in a vector so i can store them in a file
	static vector<string> _ConvertObjectToLine(vector <clsProcess>&vCompletedProcessesList) {

		string line = "";
		string IDLine = "";

		vector<string>vProcessLine;

		for (clsProcess p : vCompletedProcessesList) {
			IDLine += p._ProcessID;
		}
		vProcessLine.push_back(IDLine);

		for (clsProcess p : vCompletedProcessesList) {

			line = p._ProcessID +": " + "(" + "Response=" + to_string(p._ResponseTime) + ", " + "Turnaround=" + to_string(p._TurnaroundTime) + ", " + "Delay=" + to_string(p._Delay) + ")";
			vProcessLine.push_back(line);
		}
		return vProcessLine;
	}


	static vector<clsProcess> _LoadProcessesDataFromFile(string FileName = "ProcessInput.txt") {
		vector<clsProcess> vProcesses;
		fstream Pfile;
		string line;

		Pfile.open(FileName, ios::in);

		if (Pfile.is_open()) {

			getline(Pfile, line);//this one is to skip the first line since its just for the table namings

			//here starts the converting and filling the vector with the processes
			while (getline(Pfile, line)) {

				//reads the line form the file and converts it to an object
				clsProcess p = _ConvertLineToProcessObject(line);

				//push the object to the vector
				vProcesses.push_back(p);
			}

			Pfile.close();
		}
		return vProcesses;

	}

	

	//i made afunction for the total time requird for all processes to finish(total ticks)
	static int _TotalProcessesTimeRequired(vector<clsProcess>& vProcess) {

		int Sum = 0;

		for (int i = 0; i < vProcess.size(); i++) {

			Sum += vProcess[i]._ProcessTime;
		}

		return Sum;
	}


	//this will get the readqueue and give me the index of the process with the shortest process time
	int _GetShortestProcessTime(vector<clsProcess>& vReadQueue) {


		if (!vReadQueue.empty()) {//make sure there is something in the queue

			int MinIndex = 0;

			for (int i = 0; i < vReadQueue.size(); i++) {

				if (vReadQueue[i]._ProcessTime < vReadQueue[MinIndex]._ProcessTime) {
					MinIndex = i;
				}
			}
			return MinIndex;

		}



		else {

			return -1;


		}

	}

	//to remove the prcoess in the cpu form readqueue
	// i use it when the process enters the cpu
	//why? its non primtive algorithim, so if it enters the cpus , it stays there until it finishes
	void _RemoveProcessFromReadQueue(vector<clsProcess>& vReadQueue, int CurrentProcessIndex) {

		vReadQueue.erase(vReadQueue.begin() + CurrentProcessIndex);

	}

	
	int _CalculateTurnaroundTime() {

		return _EndTime - _ArrivalTime;

	}

	int _CalculateDelay() {

		return _TurnaroundTime - _ProcessTime;

	}

	int _CalculateResponse() {

		return _StartTime - _ArrivalTime;

	}

	//now eveyprocessshoud have everything
	void _CalculateAllStats() {
		_TurnaroundTime = _CalculateTurnaroundTime();
		_Delay = _CalculateDelay();
		_ResponseTime = _CalculateResponse();
	}

	// it just assign all the new calculations to every completed process in the completed list
	void _FinalizeStats( vector<clsProcess>&vCompletedProcessesList) {


		for (clsProcess& p : vCompletedProcessesList) {
			p._CalculateAllStats();
		}

	}

public:


	clsProcess(string id="", int ArrivalTime = 0, int ProcessTime = 0) {

		_ProcessID = id;
		_ArrivalTime = ArrivalTime;
		_ProcessTime = ProcessTime;

		NumberOfProcesses++;

	}
	
	
	
	//------------------------------------------
	void SetArriavlTime(int T) {
		_ArrivalTime = T;
	}
	int GetArrivalTime() {
		return _ArrivalTime;
	}


	void SetProcessTime(int T) {
		_ProcessTime = T;

	}
	int GetProcessTime() {
		return _ProcessTime;
	}

	void SetProcessID(string id) {
		_ProcessID = id;

	}
	string GetProcessID() {
		return _ProcessID;

	}
   //--------------------------------------------





	
	
	//this function only to make sure we are reading form the file and everything is alright
	void ProcessInfo(clsProcess p) {
		cout << "ID: " << p._ProcessID << endl;
		cout << "Art: " << p._ArrivalTime << endl;
		cout << "PT: " << p._ProcessTime << endl;

	}
	static void PrintProcessesInfo() {
		vector<clsProcess> vProcess = _LoadProcessesDataFromFile();

		for (clsProcess &p : vProcess) {

			cout << "ID: " << p.GetProcessID()<<" ";
			cout << "AT: " << p.GetArrivalTime()<<" ";
			cout << "PT: " << p.GetProcessTime() << endl;

		}

		cout << "------------------------------" << endl;


	}
	


	/*-----------------------------------------------------------------------------------------*/



	 vector<clsProcess> SJF() {

		vector<clsProcess> vProcesses = clsProcess::_LoadProcessesDataFromFile();//get the processes form the file
		vector<clsProcess> vReadQueue;
		vector<clsProcess> vCompletedProcessesList;


		//use them during the algo
		int CurrentTime = 0;
		int CompletedProcessesCount = 0;

		//i made this to i dont miss with the process time itself since i will need it during the turnaround time
		int TimeToFinish = 0;

		//i made this later so i get it one time and wont hve to search the vReadqueue more than once on one itteraiton(for preformance)
		int ShortestProcessIndex;

		clsProcess CurrentProcess;//all of my work on the process will be on this

		//wont stop until all processes are finsihed
		while (CompletedProcessesCount < vProcesses.size()) {

			//checking if arrived
			for (int i = 0; i < vProcesses.size(); i++) {
				if (vProcesses[i]._ArrivalTime == CurrentTime) {

					//arrived processes watting to get their job done in the queue
					vReadQueue.push_back(vProcesses[i]);


				}

			}

		//i changed the order here, i put cpu lock at the beggining, but this created a problem which i lost alot of time , if the cpu is locked, it skiped the other part
			//i tried this , it worked perfectly!

				if (TimeToFinish > 0) {

					TimeToFinish--;


					if (TimeToFinish == 0) {
						CurrentProcess._EndTime = CurrentTime;
						//i think unitl here we got the start time and the end time and the cpu should only hve the current working process
						// 
						// 
						//now we should put the finished process in the completetd queue and free the cpu
						vCompletedProcessesList.push_back(CurrentProcess);
						CompletedProcessesCount++;//for the loop to finish


						_IsCpuFree = 1;// now it should take anohter process
					}

				}

				//check if cpu is free
				if (_IsCpuFree) {

					//we got the process with the shortest PT
					if (!vReadQueue.empty()) {

						ShortestProcessIndex = _GetShortestProcessTime(vReadQueue);

						CurrentProcess = vReadQueue[ShortestProcessIndex];
						_RemoveProcessFromReadQueue(vReadQueue, ShortestProcessIndex);//this should remove the cureent process form the readqueue


						//gets the info for later calculations
						CurrentProcess._StartTime = CurrentTime;

						//i made this so i dont miss with PT for later Calculation(Delay and the other stuff)
						TimeToFinish = CurrentProcess._ProcessTime;

						//process took the cpu
						_IsCpuFree = 0;
						//there was a deadly mistake i did here
						//i put the _IsCpuFree outside of the bracket 
						//which resluts what if there was no process in the read queue, it will lock (take the cpu) it anyway
						// got some help from gemini to know what was wrong here
					}


				}

			



			CurrentTime++;//the actuall clock(ticks) 

		}

		_FinalizeStats(vCompletedProcessesList);//calculates everything

		return vCompletedProcessesList;//gets the vector with everything, end/start time, and all the calculations

	}


	
	//this prints everything
	void PrintFullInfo() {

		vector<clsProcess> vProcesses = SJF();

		// Define widths for the columns (easy to change later)
		const int w = 12;

		cout << "\n----------------------------------------------------------------------------------\n";
		cout << left << setw(5) << "ID"
			<< "| " << left << setw(w) << "Arrival"
			<< "| " << left << setw(w) << "ProcessTime"
			<< "| " << left << setw(w) << "TAT"
			<< "| " << left << setw(w) << "Delay"
			<< "| " << left << setw(w) << "Response"
			<< endl;
		cout << "----------------------------------------------------------------------------------\n";

		for (clsProcess& p : vProcesses) {

			cout << left << setw(5) << p._ProcessID
				<< "| " << left << setw(w) << p._ArrivalTime
				<< "| " << left << setw(w) << p._ProcessTime
				<< "| " << left << setw(w) << p._TurnaroundTime
				<< "| " << left << setw(w) << p._Delay
				<< "| " << left << setw(w) << p._ResponseTime
				<< endl;
		}
		cout << "----------------------------------------------------------------------------------\n";
	}


	//save the output on the output file
	//made it public so i decide when to save it
	//didnt make it static because if i did, i have to make half of the functions static, not sure if its a good idea
	 void SaveProcessesToFile(string FileName = "ProcessOutput.txt") {
		vector<clsProcess> vProcesses = SJF();
		vector<string>vProcessesLine=_ConvertObjectToLine(vProcesses);
		string l;
		fstream Pfile;
		Pfile.open(FileName, ios::out);


		if (Pfile.is_open()) {

			for (string line : vProcessesLine) {

				Pfile << line << endl;

			}

		}

		Pfile.close();
	}
	 void PrintOutputFileContent(string FileName = "ProcessOutput.txt") {

		 fstream Pfile;
		 string line = "";


		 Pfile.open(FileName, ios::in);


		 if (Pfile.is_open()) 
		 {
			 while (getline(Pfile, line)) {
				 cout << line << endl;
			 }
		 }


				 

			 

		 

		 Pfile.close();
	 }


};

