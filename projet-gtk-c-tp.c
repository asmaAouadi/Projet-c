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


