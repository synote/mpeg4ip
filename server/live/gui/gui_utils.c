/*
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 * 
 * The Original Code is MPEG4IP.
 * 
 * The Initial Developer of the Original Code is Cisco Systems Inc.
 * Portions created by Cisco Systems Inc. are
 * Copyright (C) Cisco Systems Inc. 2000, 2001.  All Rights Reserved.
 * 
 * Contributor(s): 
 *              Bill May        wmay@cisco.com
 */
/*
 * gui_utils.c - various gui utilities taken from Developing Linux
 * Applications and from xmps
 */
/*
 * Sample GUI application front end.
 *
 * Auth: Eric Harlow
 *
 */

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <string.h>
#include "gui_utils.h"
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
/*
 * CreateWidgetFromXpm
 *
 * Using the window information and the string with the icon color/data, 
 * create a widget that represents the data.  Once done, this widget can
 * be added to buttons or other container widgets.
 */
GtkWidget *CreateWidgetFromXpm (GtkWidget *window, gchar **xpm_data)
{
  // got this code from Xmps
    GdkColormap *colormap;
    GdkPixmap *gdkpixmap;
    GdkBitmap *mask;
    GtkWidget *pixmap;

    colormap = gtk_widget_get_colormap(window);
    gdkpixmap = gdk_pixmap_colormap_create_from_xpm_d (NULL, colormap, &mask,
						       NULL, xpm_data);
    if (gdkpixmap == NULL) {
      printf("Error\n");
      return (NULL);
    }
    pixmap = gtk_pixmap_new (gdkpixmap, mask);
    gdk_pixmap_unref (gdkpixmap);
    gdk_bitmap_unref (mask);
    gtk_widget_show(pixmap);
    return pixmap;
}



/*
 * CreateMenuItem
 *
 * Creates an item and puts it in the menu and returns the item.
 *
 * menu - container menu
 * szName - Name of the menu - NULL for a separator
 * szAccel - Acceleration string - "^C" for Control-C
 * szTip - Tool tips
 * func - Call back function
 * data - call back function data
 *
 * returns new menuitem
 */
GtkWidget *CreateMenuItem (GtkWidget *menu, 
			   GtkAccelGroup *accel_group,
			   GtkTooltips   *tooltips,
                           char *szName, 
                           char *szAccel,
                           char *szTip, 
                           GtkSignalFunc func,
                           gpointer data)
{
    GtkWidget *menuitem;

    /* --- If there's a name, create the item and put a
     *     Signal handler on it.
     */
    if (szName && strlen (szName)) {
        menuitem = gtk_menu_item_new_with_label (szName);
        gtk_signal_connect (GTK_OBJECT (menuitem), "activate",
                    GTK_SIGNAL_FUNC(func), data);
    } else {
        /* --- Create a separator --- */
        menuitem = gtk_menu_item_new ();
    }

    /* --- Add menu item to the menu and show it. --- */
    gtk_menu_append (GTK_MENU (menu), menuitem);
    gtk_widget_show (menuitem);


    /* --- If there was an accelerator --- */
    if (szAccel && accel_group != NULL) {
      guint modifier = 0;
      guint key;
      if (szAccel[0] == '^') {
	szAccel++;
	modifier |= GDK_CONTROL_MASK;
      }
      if (strncmp(szAccel, "M-", 2) == 0) {
	szAccel += 2;
	modifier |= GDK_MOD1_MASK;
      }
      key = szAccel[0];
      if (strncmp(szAccel,"<enter>", sizeof("<enter>")) == 0) {
	key = GDK_Return;
      }
      gtk_widget_add_accelerator (menuitem, 
				  "activate", 
				  accel_group,
				  key, 
				  modifier,
				  GTK_ACCEL_VISIBLE);
    }

    /* --- If there was a tool tip --- */
    if (szTip && strlen (szTip) && tooltips != NULL) {

        /* --- If tooltips not created yet --- */
        gtk_tooltips_set_tip (tooltips, menuitem, szTip, NULL);
    }

    return (menuitem);
}


/*
 * CreateMenuCheck
 *
 * Create a menu checkbox
 *
 * menu - container menu
 * szName - name of the menu
 * func - Call back function.
 * data - call back function data
 *
 * returns new menuitem
 */ 
GtkWidget *CreateMenuCheck (GtkWidget *menu, 
                            char *szName, 
                            GtkSignalFunc func, 
                            gpointer data)
{
    GtkWidget *menuitem;

    /* --- Create menu item --- */
    menuitem = gtk_check_menu_item_new_with_label (szName);

    /* --- Add it to the menu --- */
    gtk_menu_append (GTK_MENU (menu), menuitem);
    gtk_widget_show (menuitem);

    /* --- Listen for "toggled" messages --- */
    gtk_signal_connect (GTK_OBJECT (menuitem), "toggled",
                        GTK_SIGNAL_FUNC(func), data);

    return (menuitem);
}



/*
 * CreateMenuRadio
 *
 * Create a menu radio
 *
 * menu - container menu
 * szName - name of the menu
 * func - Call back function.
 * data - call back function data
 *
 * returns new menuitem
 */ 
GtkWidget *CreateMenuRadio (GtkWidget *menu, 
                            char *szName, 
                            GSList **group,
                            GtkSignalFunc func, 
                            gpointer data)
{
    GtkWidget *menuitem;

    /* --- Create menu item --- */
    menuitem = gtk_radio_menu_item_new_with_label (*group, szName);
    *group = gtk_radio_menu_item_group (GTK_RADIO_MENU_ITEM (menuitem));

    /* --- Add it to the menu --- */
    gtk_menu_append (GTK_MENU (menu), menuitem);
    gtk_widget_show (menuitem);

    /* --- Listen for "toggled" messages --- */
    gtk_signal_connect (GTK_OBJECT (menuitem), "toggled",
                        GTK_SIGNAL_FUNC(func), data);

    return (menuitem);
}


/*
 * CreateSubMenu
 *
 * Create a submenu off the menubar.
 *
 * menubar - obviously, the menu bar.
 * szName - Label given to the new submenu
 *
 * returns new menu widget
 */
GtkWidget *CreateSubMenu (GtkWidget *menubar, char *szName)
{
    GtkWidget *menuitem;
    GtkWidget *menu;
 
    /* --- Create menu --- */
    menuitem = gtk_menu_item_new_with_label (szName);

    /* --- Add it to the menubar --- */
    gtk_widget_show (menuitem);
    gtk_menu_append (GTK_MENU (menubar), menuitem);

    /* --- Get a menu and attach to the menuitem --- */
    menu = gtk_menu_new ();
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem), menu);

    /* --- Viola! --- */
    return (menu);
}


/*
 * CreateBarSubMenu
 *
 * Create a submenu within an existing submenu.  (In other words, it's not
 * the menubar.)
 *
 * menu - existing submenu
 * szName - label to be given to the new submenu off of this submenu
 *
 * returns new menu widget 
 */ 
GtkWidget *CreateBarSubMenu (GtkWidget *menu, char *szName)
{
    GtkWidget *menuitem;
    GtkWidget *submenu;
 
    /* --- Create menu --- */
    menuitem = gtk_menu_item_new_with_label (szName);

    /* --- Add it to the menubar --- */
    gtk_menu_bar_append (GTK_MENU_BAR (menu), menuitem);
    gtk_widget_show (menuitem);

    /* --- Get a menu and attach to the menuitem --- */
    submenu = gtk_menu_new ();
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem), submenu);

    /* --- Viola! --- */
    return (submenu);
}

GtkWidget *CreateOptionMenu (GtkWidget *omenu,
			     const char **names,
			     size_t max,
			     size_t current_index,
			     GtkSignalFunc on_activate)
{
  GtkWidget *menu;
  GtkWidget *menuitem;
  GSList *group;
  size_t ix;

  if (omenu != NULL) {
    if (gtk_option_menu_get_menu(GTK_OPTION_MENU(omenu)) != NULL) {
      gtk_option_menu_remove_menu(GTK_OPTION_MENU(omenu));
    }
  } else {
    omenu = gtk_option_menu_new();
  }

  menu = gtk_menu_new();
  group = NULL;
  
  for (ix = 0; ix < max; ix++) {
    menuitem = gtk_radio_menu_item_new_with_label(group, *names);
    names++;
    group = gtk_radio_menu_item_group(GTK_RADIO_MENU_ITEM(menuitem));
    gtk_menu_append(GTK_MENU(menu), menuitem);
    gtk_widget_show(menuitem);
    gtk_signal_connect(GTK_OBJECT(menuitem),
		       "activate",
		       GTK_SIGNAL_FUNC(on_activate),
		       (gpointer)ix);
  }
  gtk_option_menu_set_menu(GTK_OPTION_MENU(omenu), menu);
  gtk_option_menu_set_history(GTK_OPTION_MENU(omenu), current_index);
  gtk_widget_show(omenu);
  return (omenu);
}

void SetNumberEntryValue (GtkWidget *entry,
			     size_t value)
{
  char buffer[80];
  
  sprintf(buffer, "%u", value);
  gtk_entry_set_text(GTK_ENTRY(entry), buffer);
}

GtkWidget *AddNumberEntryBoxWithLabel (GtkWidget *vbox,
				       const char *label_name,
				       size_t value,
				       size_t value_len)
{
  GtkWidget *hbox, *label, *ret;

  hbox = gtk_hbox_new(FALSE, 1);
  gtk_widget_show(hbox);
  label = gtk_label_new(label_name);
  gtk_widget_ref(label);
  gtk_widget_show(label);
  gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 0);

  ret = gtk_entry_new_with_max_length(value_len);
  SetNumberEntryValue(ret, value);
  gtk_widget_show(ret);
  gtk_box_pack_start(GTK_BOX(hbox), ret, TRUE, TRUE, 5);

  gtk_box_pack_start(GTK_BOX(vbox),
		     hbox,
		     TRUE,
		     TRUE, 
		     3);
  return (ret);
}

int GetNumberEntryValue (GtkWidget *entry, size_t *result)
{
  const char *text;
  char *endptr;

  text = gtk_entry_get_text(GTK_ENTRY(entry));
  *result = strtoul(text, &endptr, 10);
  if (*text != '\0' && endptr != NULL && *endptr == '\0') {
    if (*result == ULONG_MAX && errno == ERANGE)
      return (0);
    return (1);
  }
  return (0);
}

GtkWidget *AddButtonToDialog (GtkWidget *dialog,
			      const char *name,
			      GtkSignalFunc on_click)
{
  GtkWidget *button;
  button = gtk_button_new_with_label(name);

  gtk_signal_connect(GTK_OBJECT(button), 
		     "clicked",
		     GTK_SIGNAL_FUNC(on_click),
		     NULL);
  GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);

    /* --- Add the button to the dialog --- */
  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->action_area),
		      button, TRUE, TRUE, 5);

  /* --- Make the button visible --- */
  gtk_widget_show (button);
  return (button);
}

GtkWidget *AddEntryBoxWithLabel (GtkWidget *vbox,
				 const char *label_name,
				 const char *initial_value,
				 size_t value_len)
{
  GtkWidget *hbox, *label, *ret;

  hbox = gtk_hbox_new(FALSE, 1);
  gtk_widget_show(hbox);
  label = gtk_label_new(label_name);
  gtk_widget_ref(label);
  gtk_widget_show(label);
  gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 0);

  ret = gtk_entry_new_with_max_length(value_len);
  gtk_entry_set_text(GTK_ENTRY(ret),
		     initial_value == NULL ? "" : initial_value);
  gtk_widget_show(ret);
  gtk_box_pack_start(GTK_BOX(hbox), ret, TRUE, TRUE, 5);

  gtk_box_pack_start(GTK_BOX(vbox),
		     hbox,
		     TRUE,
		     TRUE, 
		     3);
  return (ret);
}

/*
 * FreeChild
 *
 * Free all the children of the widget
 * This is called when the button has to display a new image.
 * The old image is destroyed here.
 */
void FreeChild (GtkWidget *widget)
{
    /* --- Free button children --- */
    gtk_container_foreach (
               GTK_CONTAINER (widget),
               (GtkCallback) gtk_widget_destroy,
               NULL);

}

void SetEntryValidator(GtkObject* object, 
	GtkSignalFunc changed_func, GtkSignalFunc leave_func)
{
	gtk_signal_connect(object,
		     "changed",
		     GTK_SIGNAL_FUNC(changed_func),
		     object);

	gtk_signal_connect(object,
		     "focus_out_event",
		     GTK_SIGNAL_FUNC(leave_func),
		     object);
}
