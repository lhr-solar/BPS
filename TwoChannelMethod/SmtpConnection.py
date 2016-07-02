import os
import sys
from smtplib import *

#used to encode the attachments
from email import encoders
from email.message import Message
from email.mime.audio import MIMEAudio
from email.mime.base import MIMEBase
from email.mime.image import MIMEImage
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText

class smtpConnection(threading.Thread): #uses threads so to not hold up any server connections or processeses

    def __init__(self, username, password, smtpServer):
        self.username = username #email username
        self.password = password #email password
        if not smtpServer.startswith("smtp."): #this needs to preceed the server name, so it adds it if the user did not
            self.smtpServer = "smtp." + smtpServer 
            
        else:
            self.smtpServer = smtpServer

        try:
            #starts up the server
            self.mail = SMTP(smtpServer, 587) 
            self.mail.ehlo() 
            self.mail.starttls()
            self.active = True #monitors whether an email connection is currently made
            
        except:
            self.active = False

        self.quit = False #if its true then the thread will end

        if self.active:
            self.quit = True
            try:
                self.mail.login(self.username, self.password)
                
            except:
                self.active = False
            
        
        
    def run(self):
        while self.quit = False:
            pass

    def encodeFileBase64(self, file, filesName): #encodes the attachment in a 64 bit data form, to get ready for attaching
        try:
            attachment = MIMEBase('application', 'octet-stream') #sets up the attachment to be in base 64
            attachment.set_payload(file.read()) #attachs tehe file it will encode
            encoders.encode_base64(attachment) #begins the encoding

            attachment.add_header('Content-Disposition', 'attachment', filename = filesName)
            file.close()

        except:
            return False
        
        return attachment 

    def attachFile(self, file, email):

        try:
            with open(file, 'rb') as f:
                email.attach(self.encodeFileBase64(f, file))
    
        except: #if the encoding didn't work than the attachments data will be turned into binary and put into a txt file, then read again from there
            try:
                with open(file, 'rb') as f:
                    dotIndex = file.index('.')
                    if dotIndex != -1:
                        file = file[:dotIndex] + "To_txt.txt"
                    else:
                        file += "To_txt.txt"

                    with open(file, 'wb') as fNew:
                        while True:
                            data = f.read(1024)
                            if data:
                                fNew.write(data)
                                continue
                            break
                        
                    email = self.attachFile(file, email, True)
                
            except:
                return False
                    
        return email

    def attachDirectory(self, directory, email, clearance = 0): #clearance of zero only attaches all files from the chosen directory, ever level of clearance declares how deep it will add directories in side this directory
        
        for file in os.listdir(directory): #list out all files and directories in the current directory for later use
            path = os.path.join(directory, file)
            
            if os.path.isfile(path): #if it is a file, run through the process of attaching it
                email = self.attachFile(path, email)

            elif clearance > 0 or clearance < 0: #clearance specifies how deep we will go into directories within the original directory, a clearance of zero means stop here, a negative clearance calls for going all the way in, no matter how deep
                if os.path.isdir(path):
                    email = self.attachDirectory(path, email, clearance - 1)

            else:
                pass
        
        return email

    def sendMail(self, sendTo, subject, text, files = None): #actually sends the email
        
        msg = MIMEMultipart() #creates an email object that will be not only just text, hence the name multipart
        #sets up the standard email header
        msg["From"] = self.username
        if isinstance(sendTo, list):
            msg["To"] = ", ".join(sendTo)
        else:
            msg["To"] = sendTo
        msg["Subject"] = subject

        #adds the text to the email
        msg.attach(MIMEText(text))

        
        if files: #if files are specified to attach than call the appropiate functions, each call updates the email object and returns it to ensure all files get attached
            if isinstance(files, list):
                for file in files:
                    if os.path.isdir(file):
                        msg = self.attachDirectory(file, msg, 1)
                        
                    elif os.path.isfile(file):
                        msg = self.attachFile(file, msg)

            else:
                if os.path.isdir(files):
                    msg = self.attachDirectory(files, msg, 1)
                        
                elif os.path.isfile(files):
                    msg = self.attachFile(files, msg)

        #heres where we actually send the email    
        try:
            self.mail.sendmail(self.username, sendTo, msg.as_string())
            return True
        
        except: 
            return False

        
if __name__ == "__main__":
    emailer = SmtpConnection('coreyohulse@gmail.com', 'corbosiny247', 'gmail.com')
    emailer.sendMail('coreyohulse@gmail.com', ['cohulse@asu.edu', 'coreyohulse@gmail.com'], 'testing', 'Hope this works!', ['serverDirectory', 'serverFile.txt'])
    print("Sent Successfully!")
