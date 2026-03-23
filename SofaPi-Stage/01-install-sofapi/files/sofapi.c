#include <gtk/gtk.h>

static gboolean launching = FALSE;

static gboolean reset_launching(gpointer data) {
    (void)data;
    launching = FALSE;
    return G_SOURCE_REMOVE;
}

static void launch_app(const char *command) {
    if (launching) return;
    launching = TRUE;

    GError *error = NULL;
    g_spawn_command_line_async(command, &error);
    if (error) {
        g_printerr("Failed to launch: %s\n", error->message);
        g_error_free(error);
        launching = FALSE;
        return;
    }

    g_timeout_add_seconds(3, reset_launching, NULL);
}

static void launch_moonlight(GtkWidget *widget, gpointer data) {
    launch_app("moonlight-qt");
}

static void launch_jellyfin(GtkWidget *widget, gpointer data) {
    launch_app("chromium-browser --kiosk --noerrdialogs --disable-infobars "
               "file:///home/pi/jellyfin-redirect.html");
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "SofaPi");
    gtk_window_fullscreen(GTK_WINDOW(window));

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        "window { background-color: #121212; }"
        "button { background-color: #2c2c2c; color: #e0e0e0;"
        "         font-size: 28px; font-weight: bold; border-radius: 12px; }"
        "button:focus, button:hover { background-color: #388E3C; }",
        -1, NULL);
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 60);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    gtk_container_add(GTK_CONTAINER(window), box);

    GtkWidget *btn_moonlight = gtk_button_new_with_label("Moonlight");
    GtkWidget *btn_jellyfin  = gtk_button_new_with_label("Jellyfin");

    gtk_widget_set_size_request(btn_moonlight, 400, 300);
    gtk_widget_set_size_request(btn_jellyfin,  400, 300);

    g_signal_connect(btn_moonlight, "clicked", G_CALLBACK(launch_moonlight), NULL);
    g_signal_connect(btn_jellyfin,  "clicked", G_CALLBACK(launch_jellyfin),  NULL);

    gtk_box_pack_start(GTK_BOX(box), btn_moonlight, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), btn_jellyfin,  TRUE, TRUE, 0);

    gtk_widget_show_all(window);

    gtk_widget_grab_focus(btn_moonlight);

    GdkWindow *gdk_win = gtk_widget_get_window(window);
    if (gdk_win) {
        GdkCursor *blank = gdk_cursor_new_for_display(
            gdk_display_get_default(), GDK_BLANK_CURSOR);
        gdk_window_set_cursor(gdk_win, blank);
        g_object_unref(blank);
    }
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.delath.sofapi",
                                              G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
