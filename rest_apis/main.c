#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <microhttpd.h>

#define PORT 8080

// Structure to hold request data for POST/PUT
struct ConnectionInfo
{
    char *data;
    size_t size;
};

// Helper function to send JSON response
static enum MHD_Result send_response(struct MHD_Connection *conn,
                                     const char *message,
                                     int status_code)
{
    struct MHD_Response *response;
    enum MHD_Result ret;

    response = MHD_create_response_from_buffer(strlen(message),
                                               (void *)message,
                                               MHD_RESPMEM_MUST_COPY);

    // Add Content-Type header
    MHD_add_response_header(response, "Content-Type", "application/json");

    ret = MHD_queue_response(conn, status_code, response);
    MHD_destroy_response(response);

    return ret;
}

// Handle GET requests
static enum MHD_Result handle_get(struct MHD_Connection *conn, const char *url)
{
    printf("GET request to: %s\n", url);

    if (strcmp(url, "/api/users") == 0)
    {
        const char *response = "{\"message\": \"List of all users\", \"users\": []}";
        return send_response(conn, response, MHD_HTTP_OK);
    }
    else if (strncmp(url, "/api/users/", 11) == 0)
    {
        const char *user_id = url + 11; // Extract ID after /api/users/
        char response[256];
        snprintf(response, sizeof(response),
                 "{\"message\": \"User details\", \"id\": \"%s\"}", user_id);
        return send_response(conn, response, MHD_HTTP_OK);
    }
    else
    {
        const char *response = "{\"error\": \"Not found\"}";
        return send_response(conn, response, MHD_HTTP_NOT_FOUND);
    }
}

// Handle POST requests
static enum MHD_Result handle_post(struct MHD_Connection *conn,
                                   const char *url,
                                   const char *upload_data,
                                   size_t upload_data_size)
{
    printf("POST request to: %s\n", url);

    if (strcmp(url, "/api/users") == 0)
    {
        if (upload_data_size > 0)
        {
            printf("Received data: %.*s\n", (int)upload_data_size, upload_data);
        }

        const char *response = "{\"message\": \"User created successfully\", \"id\": \"123\"}";
        return send_response(conn, response, MHD_HTTP_CREATED);
    }
    else
    {
        const char *response = "{\"error\": \"Invalid endpoint\"}";
        return send_response(conn, response, MHD_HTTP_BAD_REQUEST);
    }
}

// Handle PUT requests
static enum MHD_Result handle_put(struct MHD_Connection *conn,
                                  const char *url,
                                  const char *upload_data,
                                  size_t upload_data_size)
{
    printf("PUT request to: %s\n", url);

    if (strncmp(url, "/api/users/", 11) == 0)
    {
        const char *user_id = url + 11;

        if (upload_data_size > 0)
        {
            printf("Received data: %.*s\n", (int)upload_data_size, upload_data);
        }

        char response[256];
        snprintf(response, sizeof(response),
                 "{\"message\": \"User updated successfully\", \"id\": \"%s\"}", user_id);
        return send_response(conn, response, MHD_HTTP_OK);
    }
    else
    {
        const char *response = "{\"error\": \"User ID required\"}";
        return send_response(conn, response, MHD_HTTP_BAD_REQUEST);
    }
}

// Handle DELETE requests
static enum MHD_Result handle_delete(struct MHD_Connection *conn, const char *url)
{
    printf("DELETE request to: %s\n", url);

    if (strncmp(url, "/api/users/", 11) == 0)
    {
        const char *user_id = url + 11;
        char response[256];
        snprintf(response, sizeof(response),
                 "{\"message\": \"User deleted successfully\", \"id\": \"%s\"}", user_id);
        return send_response(conn, response, MHD_HTTP_OK);
    }
    else
    {
        const char *response = "{\"error\": \"User ID required\"}";
        return send_response(conn, response, MHD_HTTP_BAD_REQUEST);
    }
}

// Main request handler - called for every HTTP request
static enum MHD_Result answer_to_connection(void *cls,
                                            struct MHD_Connection *conn,
                                            const char *url,
                                            const char *method,
                                            const char *version,
                                            const char *upload_data,
                                            size_t *upload_data_size,
                                            void **con_cls)
{

    // For POST/PUT, we need to handle data upload
    if (*con_cls == NULL)
    {
        // First call for this connection
        struct ConnectionInfo *conn_info = malloc(sizeof(struct ConnectionInfo));
        if (!conn_info)
            return MHD_NO;

        conn_info->data = NULL;
        conn_info->size = 0;
        *con_cls = (void *)conn_info;

        return MHD_YES;
    }

    struct ConnectionInfo *conn_info = *con_cls;

    // Handle data upload for POST/PUT
    if (*upload_data_size != 0)
    {
        // Store the uploaded data
        conn_info->data = realloc(conn_info->data, conn_info->size + *upload_data_size + 1);
        memcpy(conn_info->data + conn_info->size, upload_data, *upload_data_size);
        conn_info->size += *upload_data_size;
        conn_info->data[conn_info->size] = '\0';

        *upload_data_size = 0; // Mark as processed
        return MHD_YES;
    }

    // Route based on HTTP method
    enum MHD_Result ret;

    if (strcmp(method, "GET") == 0)
    {
        ret = handle_get(conn, url);
    }
    else if (strcmp(method, "POST") == 0)
    {
        ret = handle_post(conn, url, conn_info->data, conn_info->size);
    }
    else if (strcmp(method, "PUT") == 0)
    {
        ret = handle_put(conn, url, conn_info->data, conn_info->size);
    }
    else if (strcmp(method, "DELETE") == 0)
    {
        ret = handle_delete(conn, url);
    }
    else
    {
        const char *response = "{\"error\": \"Method not allowed\"}";
        ret = send_response(conn, response, MHD_HTTP_METHOD_NOT_ALLOWED);
    }

    // Cleanup connection info
    if (conn_info->data)
        free(conn_info->data);
    free(conn_info);
    *con_cls = NULL;

    return ret;
}

int main()
{
    struct MHD_Daemon *daemon;

    // Start the HTTP server
    daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY,
                              PORT,
                              NULL, NULL,
                              &answer_to_connection, NULL,
                              MHD_OPTION_END);

    if (daemon == NULL)
    {
        fprintf(stderr, "Failed to start server\n");
        return 1;
    }

    printf("Server running on http://localhost:%d\n", PORT);
    printf("Available endpoints:\n");
    printf("  GET    /api/users       - List all users\n");
    printf("  GET    /api/users/:id   - Get user by ID\n");
    printf("  POST   /api/users       - Create new user\n");
    printf("  PUT    /api/users/:id   - Update user\n");
    printf("  DELETE /api/users/:id   - Delete user\n");
    printf("\nPress Enter to stop the server...\n");

    getchar(); // Wait for user input

    MHD_stop_daemon(daemon);
    return 0;
}