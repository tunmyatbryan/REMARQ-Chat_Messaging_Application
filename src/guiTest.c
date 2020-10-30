/***************************************************
 *
 * Module for Testing My WindowsRegistering a user, Window
 * 
 * Author: Michael Barbosa
 * Created: 03/01/18
 *
 * Modifications:
 	03/01/2016	MICHAEL	  Wrote main loop to open Login.c, Register.c 
 *	03/08/2018 	CEZAR	  Transfered HomeWindow from Login.c to guiTest.c
 *
 ***************************************************/

#include <gtk/gtk.h>
#include <assert.h>

#include "Register.h"
#include "Login.h"
#include "GList.h"
#include "Functions.h"
#include "DataStructures.h"

//Declare Globals
GtkWidget *HomeWindow;
GtkWidget *LoginWindow;
GtkWidget *RegisterWin;

/*exit button conditions*/
static gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	gtk_main_quit();
	return TRUE;
}

static void destroy(GtkWidget *widget, gpointer data)
{
	gtk_main_quit();
}
/*when login is clicked*/
void HometoLogin(GtkWidget *widget, gpointer data)
{

        printf("Login was clicked\n");
        gtk_widget_show_all(data);
        gtk_widget_hide(HomeWindow);

        return;
}

/*when register is clicked*/
void HometoRegister(GtkWidget *widget, gpointer data)
{
	GtkWidget *RegisterClicked;
	GtkWidget *WindowToUpdate;
	
	RegisterClicked = widget;
	WindowToUpdate  = data;
	assert(WindowToUpdate);
	
	printf("Register was clicked\n");
	if ( RegisterClicked )
	{
		gtk_widget_show_all(WindowToUpdate);
		gtk_widget_hide(HomeWindow);
	}
	
	return;
}

// Main loop for testing
int main(int argc, char *argv[] )
{
	/*variable declaration*/
	GtkWidget *LoginButton;
	GtkWidget *RegisterButton;
	GtkWidget *ExitButton;
	
	GdkColor   BackgroundColor;
	GtkWidget *ButtonContainer;
	
	LoginWindow = CreateLoginWindow(&argc, &argv);
        gtk_widget_hide(LoginWindow);
	
	RegisterWin = CreateRegisterWindow(&argc, &argv);
	gtk_widget_hide(RegisterWin);


	/*initializing gtk*/
	gtk_init(&argc, &argv);

	/*make background black*/
	BackgroundColor.red = 0;
	BackgroundColor.blue = 0;
	BackgroundColor.green = 0;

	/*create the window*/
	HomeWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);	
	gtk_window_set_title(GTK_WINDOW(HomeWindow), "Remarq - Home");
	gtk_container_set_border_width(GTK_CONTAINER(HomeWindow), 100);
	gtk_window_set_position(GTK_WINDOW(HomeWindow), GTK_WIN_POS_CENTER);
	/*set background color*/
	gtk_widget_modify_bg (HomeWindow, GTK_STATE_NORMAL, &BackgroundColor);
	

	/*setup the exit conditions for the exit button*/
	g_signal_connect(HomeWindow, "delete-event", G_CALLBACK(delete_event), NULL);
	g_signal_connect(HomeWindow, "destroy", G_CALLBACK(destroy), NULL);

	/*creating the needed boxes to put the buttons in*/
	ButtonContainer = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(HomeWindow), ButtonContainer);	
	
	/*handling login button*/
	LoginButton = gtk_button_new_with_label("Login");
	//g_signal_connect(LoginButton, "clicked", G_CALLBACK(HometoLogin), (gpointer) "LoginClicked");
	g_signal_connect(LoginButton, "clicked", G_CALLBACK(HometoLogin), LoginWindow);
	gtk_box_pack_start (GTK_BOX(ButtonContainer), LoginButton, TRUE, FALSE, 10);
	gtk_widget_show_all(LoginButton);

	/*handlilng Register Button*/
	RegisterButton = gtk_button_new_with_label("Register"); 
	//g_signal_connect(RegisterButton, "clicked", G_CALLBACK(HometoRegister), (gpointer) "RegisterClicked");
	g_signal_connect(RegisterButton, "clicked", G_CALLBACK(HometoRegister), RegisterWin);
	gtk_box_pack_start(GTK_BOX(ButtonContainer), RegisterButton, TRUE, FALSE, 10);
	gtk_widget_show_all(RegisterButton);
	
	/*handling exit button*/
	ExitButton = gtk_button_new_with_label("Exit");
	g_signal_connect_swapped(ExitButton, "clicked", G_CALLBACK(delete_event), HomeWindow);
	gtk_box_pack_start(GTK_BOX(ButtonContainer), ExitButton, FALSE, FALSE, 30);
	gtk_widget_show_all(ExitButton);

	gtk_widget_show_all(HomeWindow);
	gtk_main();
	return 0;
	
}
