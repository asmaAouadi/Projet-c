#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>

// Define a structure to hold widgets and data related to the queue.
typedef struct {
    GtkWidget *entry;         // Entry widget for user input (currently not in use)
    GtkWidget *text_view;     // Text view to display the queue contents
    GQueue *queue;            // GQueue to store integers
    GString *dequeued_items;  // String to store dequeued items (not currently used)
} QueueWidgets;

// Function to initialize the queue with random values.
static void initialize_queue_with_fixed_values(GQueue *queue) {
    for (int i = 0; i < 15; i++) {
        int *value = malloc(sizeof(int));
        if (value) {
            *value = rand() % 100; // Generate a random number between 0 and 99
            g_queue_push_tail(queue, value);
        }
    }
}

// Function to update the display of the queue in the text view.
static void update_text_view(QueueWidgets *widgets) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widgets->text_view));
    gtk_text_buffer_set_text(buffer, "", -1);  // Clear the buffer

    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);

    GList *iter = g_queue_peek_head_link(widgets->queue);
    while (iter != NULL) {
        int *value = (int *)(iter->data);
        char line[12];
        snprintf(line, sizeof(line), "%d ", *value); // Convert integer to string
        gtk_text_buffer_insert(buffer, &end, line, -1);

        iter = iter->next;
    }
}




//--------------------------------------------------------------------------------------------------------


// Callback function for the "Enqueue" button.
static void on_enqueue_clicked(GtkButton *button, gpointer user_data) {
    QueueWidgets *widgets = (QueueWidgets *)user_data;

    int *value = malloc(sizeof(int));
    if (value) {
        *value = rand() % 100; // Generate a new random number
        g_queue_push_tail(widgets->queue, value);

        update_text_view(widgets); // Refresh the display
    }
}
// Callback function for the "Dequeue" button.
static void on_dequeue_clicked(GtkButton *button, gpointer user_data) {
    QueueWidgets *widgets = (QueueWidgets *)user_data;
    int *value = g_queue_pop_head(widgets->queue);
    if (value) {
        char buffer[50];
        sprintf(buffer, "%d\n", *value);
        g_string_append(widgets->dequeued_items, buffer); // Append to dequeued items string
        free(value); // Release the memory of the dequeued value

        update_text_view(widgets); // Refresh the display
    }
}




//--------------------------------------------------------------------------------------------------------------------

int main(int argc, char **argv) {
    gtk_init(&argc, &argv);
    srand((unsigned) time(NULL)); // Initialize random number generator

    // Create a new window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_window_set_title(GTK_WINDOW(window), "Queue Example");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    // Initialize widgets structure
    QueueWidgets widgets;
    widgets.queue = g_queue_new();
    widgets.dequeued_items = g_string_new(NULL);

    initialize_queue_with_fixed_values(widgets.queue);

    // Vertical box to arrange widgets vertically
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Initialize and set up text view
    widgets.text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(widgets.text_view), FALSE);
    gtk_widget_set_margin_bottom(GTK_WIDGET(widgets.text_view), 5);
    gtk_box_pack_start(GTK_BOX(vbox), widgets.text_view, TRUE, TRUE, 1);

    // Create and configure the dequeue button
    GtkWidget *dequeue_button = gtk_button_new_with_label("Dequeue");
    g_signal_connect(dequeue_button, "clicked", G_CALLBACK(on_dequeue_clicked), &widgets);
    gtk_widget_set_margin_bottom(GTK_WIDGET(dequeue_button), 5);
    gtk_box_pack_start(GTK_BOX(vbox), dequeue_button, TRUE, TRUE, 0);

    // Create and configure the enqueue button
    GtkWidget *enqueue_button = gtk_button_new_with_label("Enqueue");
    g_signal_connect(enqueue_button, "clicked", G_CALLBACK(on_enqueue_clicked), &widgets);
    gtk_widget_set_margin_bottom(GTK_WIDGET(enqueue_button), 5);
    gtk_box_pack_start(GTK_BOX(vbox), enqueue_button, TRUE, TRUE, 0);

    update_text_view(&widgets); // Display initial queue contents

    gtk_widget_show_all(window); // Show all widgets in the window

    gtk_main(); // Main loop of the GTK application

    // Cleanup
    if (widgets.dequeued_items) {
        g_string_free(widgets.dequeued_items, TRUE);
    }
    g_queue_free_full(widgets.queue, free); // Free the queue and its contents

    return 0;
}
