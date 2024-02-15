#include <iostream>
#include <string>
#include <sstream>
#include "anotherdebugger.h"

using namespace std;

namespace anotherdebugger
{
	AnotherDebugger *g_ad;

	AnotherDebugger::AnotherDebugger(bool flag) : 
		DEBUG(flag),
		debuggeeStatus(DebuggeeStatus::NONE),
		debuggeehProcess(NULL),
		debuggeehThread(NULL),
		debuggeeprocessID(0),
		debuggeethreadID(0)
	{
		resetBreakPointHandler();

		cmdmap.insert(make_pair("s", &AnotherDebugger::onStartDebug));
		cmdmap.insert(make_pair("g", &AnotherDebugger::onGo));
		cmdmap.insert(make_pair("d", &AnotherDebugger::onDump));
		cmdmap.insert(make_pair("r", &AnotherDebugger::onShowRegisters));
		cmdmap.insert(make_pair("t", &AnotherDebugger::onStopDebug));
		cmdmap.insert(make_pair("l", &AnotherDebugger::onShowSourceLines));
		cmdmap.insert(make_pair("b", &AnotherDebugger::onSetBreakPoint));
		cmdmap.insert(make_pair("bd", &AnotherDebugger::onSetBreakPoint));
		cmdmap.insert(make_pair("in", &AnotherDebugger::onStepIn));
		cmdmap.insert(make_pair("over", &AnotherDebugger::onStepOver));
		cmdmap.insert(make_pair("out", &AnotherDebugger::onStepOut));
		cmdmap.insert(make_pair("llv", &AnotherDebugger::onShowLocalVariables));
		cmdmap.insert(make_pair("lgv", &AnotherDebugger::onShowGlobalVariables));
		cmdmap.insert(make_pair("st", &AnotherDebugger::onShowStackTrace));
	}

	void AnotherDebugger::startDebuggerLoop()
	{
		cout << "Another Debugger by lyyyuna" << endl;
		
		if (g_ad != this)
		{
			cout << "Should first init the global AnotherDebugger pointer" << endl;
			return;
		}

		string cmdline;

		while (true)
		{
			cout << endl << "> ";
			getline(cin, cmdline);

			parseCommand(cmdline);
			if (false == dispatchCommand())
			{
				break;
			}

			cleanCommand();
		}
	}

	void AnotherDebugger::parseCommand(string & cmdline)
	{
		istringstream cmdstrStream(cmdline);

		string arg;
		while (cmdstrStream >> arg)
		{
			cmds.push_back(arg);
		}
	}

	bool AnotherDebugger::dispatchCommand()
	{
		if (cmds.size() == 0)
		{
			cout << "Invalid command." << endl;
			return true;
		}

		if ("q" == cmds[0])
		{
			return false;
		}


		auto cmditer = cmdmap.find(cmds[0]);
		if (cmditer == cmdmap.end())
		{
			cout << "Invalid command." << endl;
		}
		else
		{
			auto handler = cmditer->second;
			(this->*handler)(cmds);
		}

		return true;
	}
}