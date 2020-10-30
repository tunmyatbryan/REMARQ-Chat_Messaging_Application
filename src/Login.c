#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <assert.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>

#include "Login.h"
#include "Register.h"
#include "GList.h"

/*variable decleration*/
GtkWidget *LoginExit;		/*Creating a login exit button*/
GtkWidget *LoginButton;

GtkWidget *TopBox;                /*Topbox will contain other boxes for Login window*/
GtkWidget *RegisterHBox1;	
GtkWidget *RegisterHBox2;
GtkWidget *LoginWindow;        /*second window variable*/
GtkWidget *UserIDEntry;		/*Login User ID text Entry variable*/
GtkWidget *UserPasswordEntry;	/*Login User Password text entry variable*/
GtkWidget *UserIDFrame;
GtkWidget *UserPasswordFrame;
GtkWidget *UserIDLabel;
GtkWidget *UserPasswordLabel;

/*error handeling*/
GtkWidget *ErrorLabel;
GtkWidget *SuccessLabel;
GtkWidget *ErrorTable;

/*entry temps*/
gint tmp_pos;
gint tmp_pos1;			

/*background image*/
GtkWidget *RemarqImage;
GtkWidget *Layout;


/*entry text variables*/
const gchar *UserLoginCheck;
const gchar *PasswordLoginCheck;

/*move to Main window*/
GtkWidget *MainWindow;

/*sets up exit button conditions*/
static gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	g_print("delete event occured\n");
	gtk_main_quit();
	return TRUE;
}

static void destroy(GtkWidget *widget, gpointer data)
{
	gtk_main_quit();
}


/* "Moving in between windows"*/

/*Login to home window*/
static void LogintoMain(GtkButton *widget, GObject *context_object)
{

	GtkWidget *accept_window = g_object_get_data(context_object, "MainWindow");
        GtkEntry *accept_User = g_object_get_data(context_object, "UserIDEntry");
        GtkEntry *accept_Pass = g_object_get_data(context_object, "UserPasswordEntry");

        GdkWindow *MainWindow_in = gtk_widget_get_window(accept_window);
        const char *UserEntry_in = gtk_entry_get_text(accept_User);
	const char *tempPassEntry_in = gtk_entry_get_text(accept_Pass);

	char *PassEntry_in = (char*)tempPassEntry_in;
	

	/*Handling the error display on failed login*/	
	ErrorLabel = gtk_label_new("ERROR - Login Failed, Incorrect Username or Password");
	gtk_table_attach_defaults(GTK_TABLE(ErrorTable), ErrorLabel  , 0, 1, 0, 1);
	gtk_widget_show(ErrorLabel);	
	
	
	GdkColor ErrorColor;
	ErrorColor.red = 0xffff;
	ErrorColor.blue = 0;
	ErrorColor.green = 0;

	GdkColor SuccessColor;
	SuccessColor.red = 0;
	SuccessColor.blue = 0;
	SuccessColor.green = 0xffff;	
	
	gtk_widget_modify_fg(ErrorLabel, GTK_STATE_NORMAL, &ErrorColor);
	
	/*form of "LOGIN USERNAME "*/
        int n, serversock, portno;
        char str[256];
       	bzero(str,255);
        char response[256];
        struct hostent *server;
        struct sockaddr_in serv_addr;
        strcpy(str, "LOGIN ");
        strcat(str, UserEntry_in);
        strcat(str, " ");
	/* adding the encrypted "Password" */
	unsigned int i;
        for(i = 0; i < strlen(PassEntry_in); ++i)
        {
                PassEntry_in[i] = PassEntry_in[i] - 10;
        }
        strcat(str, PassEntry_in);

	/*Connect to server attempt*/
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

	/*Send the login string to the server*/
	printf("DEBUG Login Token is: %s \n", str);
	n = write(serversock, str, strlen(str));
	if(n<0)
		error("ERROR writing to server");
	printf("DEBUG sent login token to server\n");

	/*Wait for response from the server*/
	n = read(serversock, response, 255);
	if(n<0)
		error("ERROR reading from server");
	printf("DEBUG recieved response from server\n");
	printf("DEBUG response from server: %s \n", response);

	/*Hande response from server*/
	if(strncmp(response, "ERROR", strlen("ERROR")) == 0)
		{
			/*LOGIN FAILED*/
			//gtk_widget_show(ErrorLabel);

		}
	else if (strncmp(response, "SUCCESS", strlen("SUCCESS")) == 0)
		{
			/*LOGIN SUCCESSFUL*/
			/*RECIEVE USERDATA FILE FROM SERVER*/
			if(GetUserNametxt(serversock) > 0)
				error("ERROR on recieving data from server");
			printf("Recieved Userdata.txt\n");
			/*display labels*/

			gtk_widget_hide(ErrorLabel);

			/*move to to the next window*/
			assert(accept_window);
        		gtk_widget_hide(LoginWindow);
        		gtk_widget_show_all(accept_window);
       			gtk_widget_hide(LoginWindow);
		}

	/*assert(data);*/
/*	assert(accept_window);
	gtk_widget_hide(LoginWindow);
*/	/*gtk_widget_show_all(data);*/
/*	gtk_widget_show_all(accept_window);
        gtk_widget_hide(LoginWindow);
*/
}

/*setup conditions for entry texts*/

/*This keeps track of what the user is typing for Username*/
static void UserLogin(GtkWidget *widget, GtkWidget *entry)
{
	UserLoginCheck = gtk_entry_get_text (GTK_ENTRY(entry));
	/*printf("Entry Contents: %s\n", UserLoginCheck);*/
}

/*This keeps track of what the user is typing for Password*/
static void PasswordLogin(GtkWidget *widget, GtkWidget *entry)
{
	PasswordLoginCheck = gtk_entry_get_text (GTK_ENTRY(entry));
        /*printf("Entry Contents: %s\n", PasswordLoginCheck);*/
}


GtkWidget *CreateLoginWindow(int *argc, char **argv[]) /*parameters that gets passed in(mainly for GTK behind scenes)*/
{
	/*case to move to Main window*/
	MainWindow = CreateListWin(argc, argv);
	gtk_widget_hide(MainWindow);

	gtk_init(argc, argv);		/*Sets default stuff, mostly used for gtk - required for every GUI*/


	/*changing the color of the background*/
	GdkColor color;
	GdkColor LabelColor;

	/* making color have the value of black*/
	color.red = 21588;		/*Set the RGB Values*/
	color.green = 21588;	/*make sure they are 16-bit numbers*/
	color.blue = 21588;

	/*make LabelColor have the value of white*/
	LabelColor.red = 0xffff;
	LabelColor.blue = 0xffff;
	LabelColor.green = 0xffff;
	
	/*Login window stuff*/
	LoginWindow =  gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(LoginWindow), GTK_WIN_POS_CENTER);
        gtk_window_set_title(GTK_WINDOW(LoginWindow), "Remarq - Login");    /*set a title to the window*/
        gtk_widget_modify_bg (LoginWindow, GTK_STATE_NORMAL, &color);   /* this changes the background to the value of color*/
        gtk_container_set_border_width(GTK_CONTAINER(LoginWindow), 100); /*border size*/
                                
	/*set up the exit conditions for the exit buttons*/
	g_signal_connect(LoginWindow, "delete-event", G_CALLBACK(delete_event), NULL);
	g_signal_connect(LoginWindow, "destroy", G_CALLBACK(destroy), NULL);

	
	/*create needed boxes*/
	TopBox = gtk_vbox_new(FALSE, 0);
        gtk_container_add(GTK_CONTAINER(LoginWindow), TopBox);
	RegisterHBox1 = gtk_hbox_new(FALSE, 5);
	gtk_container_add(GTK_CONTAINER(TopBox), RegisterHBox1);
	RegisterHBox2 = gtk_hbox_new(FALSE, 5);
	gtk_container_add(GTK_CONTAINER(TopBox), RegisterHBox2);

	/*Create the User ID Label*/
	UserIDLabel = gtk_label_new ("Enter ID: ");
	gtk_widget_modify_fg (UserIDLabel, GTK_STATE_NORMAL, &LabelColor);
	gtk_box_pack_start(GTK_BOX(RegisterHBox1), UserIDLabel, FALSE, FALSE, 0);
	
	
	/*create the User ID Entry box*/
	UserIDEntry = gtk_entry_new();	/*create the entry box*/
	gtk_entry_set_max_length (GTK_ENTRY(UserIDEntry), 50); /*max length 255*/
	g_signal_connect (UserIDEntry, "changed", G_CALLBACK(UserLogin), UserIDEntry);
	gtk_entry_set_text (GTK_ENTRY (UserIDEntry), "MAX");
	tmp_pos = GTK_ENTRY (UserIDEntry)->text_length;
	gtk_editable_insert_text (GTK_EDITABLE(UserIDEntry), " 50", -1, &tmp_pos );
	gtk_editable_select_region (GTK_EDITABLE (UserIDEntry), 0, GTK_ENTRY(UserIDEntry)->text_length);
	gtk_box_pack_start(GTK_BOX(RegisterHBox1), UserIDEntry, TRUE, TRUE, 0);
		
		
	/*Create the Password Label*/
	UserPasswordLabel = gtk_label_new("Enter Password: ");
	gtk_widget_modify_fg (UserPasswordLabel, GTK_STATE_NORMAL, &LabelColor);
	gtk_box_pack_start(GTK_BOX(RegisterHBox2), UserPasswordLabel, FALSE, FALSE, 0);
		
	/*Create the Password Entry box*/
        UserPasswordEntry = gtk_entry_new();
        gtk_entry_set_max_length (GTK_ENTRY(UserPasswordEntry), 50);
        //g_signal_connect(UserPasswordEntry, "changed", G_CALLBACK(PasswordLogin), UserPasswordEntry);
        gtk_entry_set_text (GTK_ENTRY(UserPasswordEntry), "MAX");
        tmp_pos1 = GTK_ENTRY(UserPasswordEntry)->text_length;
        gtk_editable_insert_text(GTK_EDITABLE(UserPasswordEntry), " 50", -1, &tmp_pos1);
        gtk_editable_select_region (GTK_EDITABLE(UserPasswordEntry), 0, GTK_ENTRY(UserPasswordEntry)->text_length);
	gtk_entry_set_visibility(GTK_ENTRY (UserPasswordEntry), FALSE);
        gtk_box_pack_start (GTK_BOX(RegisterHBox2), UserPasswordEntry, TRUE, TRUE, 0);
		

	/*Make the login button on the Login window*/	
	LoginButton = gtk_button_new_with_label("Login");
	//g_signal_connect(LoginButton, "clicked", G_CALLBACK (LogintoHome), (gpointer) "LogintoHome");
	g_object_set_data(G_OBJECT(LoginButton), "MainWindow", MainWindow);
        g_object_set_data(G_OBJECT(LoginButton), "UserIDEntry", UserIDEntry);
        g_object_set_data(G_OBJECT(LoginButton), "UserPasswordEntry", UserPasswordEntry);

	g_signal_connect(GTK_BUTTON(LoginButton), "clicked", G_CALLBACK (LogintoMain), LoginButton);
	gtk_box_pack_start(GTK_BOX(TopBox), LoginButton, TRUE, FALSE, 10);	
		
		
	/*Make the exit button for the Login window*/
	LoginExit = gtk_button_new_with_label("EXIT");
	g_signal_connect_swapped(LoginExit, "clicked", G_CALLBACK(gtk_widget_destroy), LoginWindow);
	gtk_box_pack_start (GTK_BOX(TopBox), LoginExit, TRUE, FALSE, 10);

	/*set up the error table*/
        ErrorTable = gtk_table_new(1,1, TRUE);
        gtk_table_set_homogeneous(GTK_TABLE(ErrorTable), TRUE);
        //gtk_container_add(GTK_CONTAINER(TopBox),ErrorTable);
        gtk_box_pack_start(GTK_BOX(TopBox), ErrorTable, FALSE, FALSE, 0);
                

	/*dont forget to make the box itself visible*/
	gtk_widget_show_all (LoginWindow);			/*displays the window*/

	//gtk_main ();					/*sleeps for x events, waiting for actions like button presses*/
	return LoginWindow;
}


#ifdef LMAIN
int main(int argc, char *argv[])
{
	GtkWidget* Window;
	printf("we got this far\n");
	Window = CreateLoginWindow(&argc, &argv);
       	if( !Window )
       	{
         	printf("Cannot Open GUI Window HA IT FAILED LOSER!");
            	//exit(10);
            	//gtk_main();
		return 0;
	}
}	 
#endif

