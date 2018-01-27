#! /usr/bin/python

import wx #, wx.lib.scrolledpanel
import sys
import serial, serial.tools.list_ports

ID_FILE       = wx.NewId()
ID_ABOUT      = wx.NewId()
ID_IMPORT     = wx.NewId()
ID_EXIT       = wx.NewId()

####################################################################################################
#      Main panel - place buttons and indicators here
####################################################################################################
class _MainPanel(wx.Panel):
	def __init__(self, parent, config):
		wx.Panel.__init__(self,parent=parent, id=wx.ID_ANY)

		panelSizer = wx.BoxSizer(wx.VERTICAL)
		targetSizer = wx.BoxSizer(wx.HORIZONTAL)
		consoleSizer = wx.BoxSizer(wx.VERTICAL)

		self.serialConnection = None

		self.tenHztimer = wx.Timer(self,100)
		self.Bind(wx.EVT_TIMER,self.tenHz_SIM_Cyclic_Routine,self.tenHztimer)
		#  timer interval is 100  milliseconds
		self.tenHztimer.Start(100)

		self.font1 = wx.Font(10, wx.MODERN, wx.NORMAL, wx.NORMAL, False, u'Courier New')

		comChoices = []
		for thing in serial.tools.list_ports.comports():
			comChoices.append(thing.device)
		sys.stdout.write(''.join(comChoices))
		self.comBx = wx.ComboBox(self, choices=comChoices, value=comChoices[0], size=(150, 30))
		self.comBx.SetFont(self.font1)
		targetSizer.Add(self.comBx, 0, wx.ALL, 5)
		self.openCOMButton = wx.Button(self, label="Open COM", size=(100,30))
		self.Bind(wx.EVT_BUTTON, self.openCOM, self.openCOMButton)
		targetSizer.Add(self.openCOMButton, 0, wx.ALL, 5)

		consoleLabel = wx.StaticText(self, -1, "Console Output")
		consoleSizer.Add(consoleLabel, 0, wx.ALL, 2)
		self.consoleout = wx.TextCtrl(self, size=(500,100), style=wx.TE_MULTILINE | wx.TE_READONLY )
		self.consoleout.SetFont(self.font1)
		consoleSizer.Add(self.consoleout, 1, wx.ALL|wx.EXPAND, 2)

		panelSizer.Add(targetSizer, 0, wx.ALL|wx.EXPAND,5)
		panelSizer.Add(consoleSizer, 1, wx.ALL|wx.EXPAND,5)

		self.SetSizer(panelSizer)

	def openCOM(self,event):
		self.serialConnection = serial.Serial(self.comBx.GetValue(), 9600)

	''' tenHz_SIM_Cyclic_Routine:
        So why do each of the tab panels have this tenHz Cyclic Routine?
        Because of a wxPython thing. Only the main thread, the gui thread, may change display. Trying to write to the
        gui console or something like that using any other thread may cause a seg fault. At the very least, you'd get 
        the error "Gtk-CRITICAL **: gtk_text_layout_real_invalidate: assertion 'layout->wrap_loop_count == 0'". What 
        this error is saying is that wxPython is pissed because some of the gui stuff was modified without it knowing 
        about it. 
        Solution: have the main thread go off into this periodic routine (I chose 10Hz) to periodically push the updates
        that any of the threads want to make to the gui display. So what you do is have the side thread go push the info
        off into a queue, and then this tenHz cyclic routine will pull that and write to the gui. Emily '''
	def tenHz_SIM_Cyclic_Routine(self, event):
		if self.serialConnection:
			line = self.serialConnection.read(self.serialConnection.in_waiting)
			line = line.decode("utf-8")
			self.consoleout.AppendText(line)

####################################################################################################
#      Main frame of window
####################################################################################################
class UTSVTMainFrame(wx.Frame):
	def __init__(self,parent,id):
		wx.Frame.__init__(self,parent,id, "University of Texas Solar Car Team (UTSVT)", size=(800,500))
		self.Bind(wx.EVT_CLOSE, self.closeWindow)

		#self.panel = wx.lib.scrolledpanel.ScrolledPanel(parent=self, id=-1)
		#self.panel.SetupScrolling()
		self.panel = _MainPanel(self, parent)

		self.Bind(wx.EVT_CLOSE, self.closeWindow)

####################################################################################################
#       status bar at the bottom
####################################################################################################
		status=self.CreateStatusBar()

####################################################################################################
#       setting up menu
####################################################################################################
		filemenu=wx.Menu()
		filemenu.Append(ID_IMPORT, "&Import", "Import a script from a local location")
		filemenu.Append(ID_ABOUT, "&About", "About the AA2 project")
		filemenu.Append(ID_EXIT, "&Exit", "Close this application")

		menubar = wx.MenuBar()
		menubar.Append(filemenu, "&File")

		self.SetMenuBar(menubar)

		wx.EVT_MENU(self, ID_ABOUT, self.onAbout)
		wx.EVT_MENU(self, ID_EXIT, self.onExit)
		wx.EVT_MENU(self, ID_IMPORT, self.onImport)

		self.Show(True)

####################################################################################################
#       When about select 
####################################################################################################
	def onAbout(self,event):
		d= wx.MessageDialog(self, "                              \nThe Team:\nCorey \nEmily \nArmin \nArtin \nAli \nRichard \nAlto \nSijin \nCole", "About UTSVT", wx.OK)
		d.ShowModal()
		d.Destroy()

####################################################################################################
#       When exit selected
####################################################################################################
	def onExit(self,event):      
		self.Close(True)

####################################################################################################
#       When Import selected
####################################################################################################
	def onImport(self,event):
		scriptbox=wx.TextEntryDialog(self, "Type the name of the file of the script to load","Load Script","")
		if scriptbox.ShowModal()==wx.ID_OK:
			script=scriptbox.GetValue()

####################################################################################################
#       After close selected
####################################################################################################
	def closeWindow(MainApp, event):
		MainApp.Destroy()

####################################################################################################      
#   Main Application      
####################################################################################################
class MainApp(wx.App):

	def OnInit(self):
		#self.Init()  # initialize the inspection tool
		self.frame = UTSVTMainFrame(None, -1)
		self.frame.Show(True)
		self.SetTopWindow(self.frame)

		return True

####################################################################################################           
#       main loop
####################################################################################################
if __name__=="__main__":
	app=MainApp(0)
	app.MainLoop()