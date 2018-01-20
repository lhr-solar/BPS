#! /usr/bin/python

import wx #, wx.lib.scrolledpanel

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
		push1Sizer = wx.BoxSizer(wx.HORIZONTAL)
		push2Sizer = wx.BoxSizer(wx.HORIZONTAL)

		self.words = wx.StaticText(self, -1, "Hi. I am words.")
		self.push1button = wx.Button(self, label="Press me!")
		self.Bind(wx.EVT_BUTTON, self.push1, self.push1button)
		self.push1ctrl = wx.TextCtrl(self)
		self.push2button= wx.Button(self, label="Press me!")
		self.Bind(wx.EVT_BUTTON, self.push2, self.push2button)
		self.push2ctrl = wx.TextCtrl(self)

		push1Sizer.Add(self.push1button)
		push1Sizer.Add(self.push1ctrl, 0, wx.EXPAND)
		push2Sizer.Add(self.push2button)
		push2Sizer.Add(self.push2ctrl, 0, wx.EXPAND)

		panelSizer.Add(self.words)
		panelSizer.Add(push1Sizer, 0, wx.EXPAND)
		panelSizer.Add(push2Sizer, 0, wx.EXPAND)

		self.SetSizer(panelSizer)

	def push1(self, parent):
		self.push1ctrl.SetValue("Pushed!")
	def push2(self, parent):
		self.push2ctrl.SetValue("Pushed!")

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