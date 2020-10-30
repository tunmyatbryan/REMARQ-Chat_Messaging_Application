/***************************************************
 * Module for Registering a user, Window
 * 
 * Author: Michael Barbosa
 * Created: 02/24/18
 *
 * Modifications:
 * 03/01/18	MICHAEL		Created main for DEBUG purposes
 * 03/11/18	JOSH		Merged client side server connection into CREATEbutton_callback()
 * 03/11/18	MICHAEL		modded CREATEButton_callback() to accept multiple args
 *
 ***************************************************/
#include <gtk/gtk.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <fcntl.h>
#include "Register.h"
#include "Login.h"
#include "GList.h"
#include "Functions.h"

// Window variables
GtkWidget *RegisterWindow;

// Table variables
GtkWidget *Regtable;

// Button variables
GtkWidget *CREATEButton, *EXITButton;

// Label Variables
GtkWidget *RegisterUser, *UserID, *Pass, *ERRORLabel, *F_Name, *L_Name, *DOB;

// TextEntry Variables
GtkWidget *UserEntry, *PassEntry, *PassConfirmEntry, *F_NameEntry, *L_NameEntry, *DOBEntry;

//Moving to ChatWindow
GtkWidget *ChatWindow;

/*Error Label*/
GtkWidget *ErrorLabel;

// Close Window event
static gboolean on_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	g_print("delete event occured\n");
	gtk_main_quit();
	return TRUE;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

// CancelButton Callback definition
static EXITButton_callback(GtkWidget *button, gpointer data)
{
	printf("Remarq is EXITING...\n");
	gtk_main_quit();
}

// CreateButton Callback definition
void CREATEButton_callback(GtkButton *button, GObject *context_object)
{
	// moving From registerWindow to ChatWindow
   	GtkWidget *RegisterClicked;
	GtkWidget *WindowToUpdate;

	//Entry parameters that get read in
	GtkWidget *accept_Window = g_object_get_data(context_object, "ChatWindow");
	GtkEntry *accept_User    = g_object_get_data(context_object, "UserEntry");
	GtkEntry *accept_Pass    = g_object_get_data(context_object, "PassEntry");
	GtkLabel *accept_Error   = g_object_get_data(context_object, "ERRORLabel");
		
	// accepting the parameters
	GdkWindow *ChatWindow_in = gtk_widget_get_window(accept_Window);
	const char *UserEntry_in = gtk_entry_get_text(accept_User);
	const char *tempPassEntry_in = gtk_entry_get_text(accept_Pass);
	
	/*handeling the error label*/
	ErrorLabel = gtk_label_new("Unable to register");
	gtk_table_attach_defaults(GTK_TABLE(Regtable), ErrorLabel  , 0, 10, 3, 4);
	gtk_widget_show(ErrorLabel);

	int n, serversock, portno;
	char *PassEntry_in = (char*)tempPassEntry_in;
	
	char str[256];
	bzero(str,255);
	char response[256];
	struct sockaddr_in serv_addr;
	struct hostent *server;

	/*Construct Register token*/
	strcpy(str, "REGISTER ");
	strcat(str, UserEntry_in);
	strcat(str, " ");
	unsigned int i;
    	for(i = 0; i < strlen(PassEntry_in); ++i)
    	{
        	PassEntry_in[i] = PassEntry_in[i] - 10;
    	}
	strcat(str, PassEntry_in);

	/*Attempt connection to server*/
	portno = 5280;
	serversock = socket(AF_INET, SOCK_STREAM, 0);
	if(serversock<0)
		error("ERROR opening socket");
	server = gethostbyname("laguna");
	if (server == NULL)
		error("ERROR on host connect");
	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
   bcopy((char *)server->h_addr, 
   	(char *)&serv_addr.sin_addr.s_addr,
   	server->h_length);
   serv_addr.sin_port = htons(portno);
   
   
 	if (connect(serversock,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
  		error("ERROR connecting");
  		
	printf("Here is the str: %s\n",str);
	
	/*Send register token to the server*/
	n = write(serversock, str, strlen(str));
	if(n<0)
		error("ERROR writing to server");

	/*wait for server response*/
	n = read(serversock, response, 255);
	if(n<0)
		error("ERROR reading from server");

	printf("DEBUG The response from the server was: %s \n", response);

	/*Handle server response*/
	if(strncmp(response, "ERROR", strlen("ERROR")) == 0)
	{
		/*REGISTRATION FAILED */	
		printf("Server Returned Error...");
	}
	else if(strncmp(response, "SUCCESS", strlen("SUCCESS")) == 0)
	{	
		printf("server returned sucessful...");
		/*REGISTRATION SUCCESSFUL*/
		printf("The Create Button Was pressed...\n Account Registration Successful...");

		/*RECEIVE USERDATA FILE FROM SERVER*/
		if(GetUserNametxt(serversock) > 0)
			error("ERROR on recieving data from server");
		printf("Recieved Userdata.txt\n");

		/*PROCEED TO NEXT WINDOW*/
		assert(accept_Window);
		gtk_widget_show_all(accept_Window);
		gtk_widget_hide(RegisterWindow);
		ServerRun();	
	}
	else
	{
		printf("You made it this far...");
	}
		
	//RegisterClicked = widget;
	//WindowToUpdate  = data;	
}

// Create Register Window
GtkWidget *CreateRegisterWindow( int *argc, char **argv[] )
{
	//case to move to ChatWindow
	ChatWindow = CreateListWin(argc, argv);
	gtk_widget_hide(ChatWindow);

	gtk_init(argc, argv);

	RegisterWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(RegisterWindow, 400, 500);
	gtk_container_set_border_width(GTK_CONTAINER(RegisterWindow), 5);
	gtk_window_set_position(GTK_WINDOW(RegisterWindow), GTK_WIN_POS_CENTER);
	gtk_window_set_title(GTK_WINDOW(RegisterWindow),"Remarq Account Registration");
	gtk_window_set_resizable(GTK_WINDOW(RegisterWindow),TRUE);

	// create table
	Regtable = gtk_table_new(10,10, TRUE);
	gtk_table_set_homogeneous(GTK_TABLE(Regtable), TRUE);
	gtk_container_add(GTK_CONTAINER(RegisterWindow),Regtable);
	
	// creating labels
	RegisterUser = gtk_label_new("Register Account");
	UserID = gtk_label_new("User ID:");
	Pass = gtk_label_new("Password:");
	F_Name = gtk_label_new("First Name:");	
	L_Name = gtk_label_new("Last Name:");	
	DOB = gtk_label_new("Date of Birth:");
	
// Create the error label for Already Registered users
	ERRORLabel = gtk_label_new("");

	// attach labels to window
	gtk_table_attach_defaults(GTK_TABLE(Regtable), RegisterUser, 0, 10, 0, 1);
	gtk_table_attach_defaults(GTK_TABLE(Regtable), UserID      , 1,  3, 1, 2);
	gtk_table_attach_defaults(GTK_TABLE(Regtable), Pass        , 1,  3, 2, 3);
	//gtk_table_attach_defaults(GTK_TABLE(Regtable), ERRORLabel  , 0, 10, 3, 4);


	// creating text entries
	UserEntry = gtk_entry_new();
	PassEntry = gtk_entry_new();
	gtk_entry_set_visibility(GTK_ENTRY(PassEntry), FALSE);
	PassConfirmEntry = gtk_entry_new();
	gtk_entry_set_visibility(GTK_ENTRY(PassConfirmEntry), FALSE);
	F_NameEntry = gtk_entry_new();
	L_NameEntry = gtk_entry_new();
	DOBEntry    = gtk_entry_new();

	// attach text entries to window
	gtk_entry_set_max_length(GTK_ENTRY(UserEntry), 20); // limits char count
	gtk_entry_set_max_length(GTK_ENTRY(PassEntry), 20); // limits char count
	gtk_table_attach_defaults(GTK_TABLE(Regtable), UserEntry, 3, 8, 1, 2);
	gtk_table_attach_defaults(GTK_TABLE(Regtable), PassEntry, 3, 8, 2, 3);
	
	//creating buttons
	CREATEButton = gtk_button_new_with_label("Create");
	EXITButton   = gtk_button_new_with_label("Exit"  );

	// attach buttons to window
	gtk_table_attach_defaults(GTK_TABLE(Regtable), CREATEButton, 7, 9, 8, 9 );
	gtk_table_attach_defaults(GTK_TABLE(Regtable), EXITButton,   1, 3, 8, 9 );
	
	// Delete Window event
	g_signal_connect(RegisterWindow, "delete_event", G_CALLBACK(on_delete_event), NULL);
	gtk_widget_set_events(RegisterWindow, GDK_BUTTON_PRESS_MASK);
	
	g_object_set_data(G_OBJECT(CREATEButton), "ChatWindow", ChatWindow);
	g_object_set_data(G_OBJECT(CREATEButton), "UserEntry" , UserEntry );
	g_object_set_data(G_OBJECT(CREATEButton), "PassEntry" , PassEntry );
	
	g_signal_connect(GTK_BUTTON(CREATEButton), "clicked", G_CALLBACK(CREATEButton_callback), CREATEButton);
	g_signal_connect(EXITButton, "clicked", G_CALLBACK(EXITButton_callback), NULL);


	gtk_widget_show_all(RegisterWindow);
	//gtk_main();

	return RegisterWindow;
}


// Main LOOP FOR TESTING
#ifdef RMAIN
int main(int argc, char *argv[] )
{
	GtkWidget *Window;

	printf("This program is running from main loop test file/n");
	
	// Call the correct windows in order
	Window = CreateRegisterWindow(&argc, &argv);
	if( !Window )
	{
		printf("Cannot Open GUI Window HA IT FAILED LOSER!");
		//exit(10);
	}
	
	gtk_main();
	return 0;
}
#endif

