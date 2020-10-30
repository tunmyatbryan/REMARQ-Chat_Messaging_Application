/***************************************************
 * Register.h
 * Header file for Register.c
 * 
 * Author: Michael Barbosa
 * Created: 03/01/18
 *
 * Modifications:
 *
 ***************************************************/

#ifndef REGISTER_H
#define REGISTER_H


// Closing window callback definition
static gboolean on_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data);

// Cancel button callback definition
static CANCELButton_callback(GtkWidget *button, gpointer data);

// Create button callback definition
void CREATEButton_callback(GtkButton *button, GObject *context_object);

// Creating window function
GtkWidget *CreateRegisterWindow( int *argc, char **argv[] );

#endif
