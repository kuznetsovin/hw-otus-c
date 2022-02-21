#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../assets/cJSON/cJSON.h"

const char *LOCATION_URL = "https://www.metaweather.com/api/location/";
const size_t URL_LEN = 100;

struct Response {
    char *body;
    size_t size;
};

/*
 * Extract location identifier from api request.
 *
 * Example request format:
 * [
 *      {
 *          "title":"Moscow",
 *          "location_type":"City",
 *          "woeid":2122265,
 *          "latt_long":"55.756950,37.614971"
 *       }
 * ]
 */
size_t extract_woeid(struct Response *r) {
    cJSON *city_json = cJSON_Parse(r->body);

    if (city_json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        exit(EXIT_FAILURE);
    }

    cJSON *location = NULL;

    size_t location_id = 0;
    cJSON_ArrayForEach(location, city_json) {
        cJSON *woeid = cJSON_GetObjectItemCaseSensitive(location, "woeid");

        if (!cJSON_IsNumber(woeid)) {
            fprintf(stderr, "Invalid woeid value");
            exit(EXIT_FAILURE);
        }

        location_id = woeid->valueint;
    }

    if (location_id == 0) {
        fprintf(stderr, "Valid location was not found");
        exit(EXIT_FAILURE);
    }

    cJSON_Delete(city_json);
    return location_id;
}

void print_weather_today(struct Response *r) {
    cJSON *weather_info = cJSON_Parse(r->body);

    if (weather_info == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Request parse err: %s\n", error_ptr);
        }
        exit(EXIT_FAILURE);
    }

    cJSON *weather = cJSON_GetObjectItemCaseSensitive(weather_info, "consolidated_weather");
    if (weather == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Consolidated weather section parse err: %s\n", error_ptr);
        }
        exit(EXIT_FAILURE);
    }

    cJSON *weather_today = cJSON_GetArrayItem(weather, 0);
    if (weather_today == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Today weather parse err: %s\n", error_ptr);
        }
        exit(EXIT_FAILURE);
    }

    cJSON *weather_state = cJSON_GetObjectItemCaseSensitive(weather_today, "weather_state_name");
    if (!cJSON_IsString(weather_state)) {
        fprintf(stderr, "Invalid weather_state value");
        exit(EXIT_FAILURE);
    }
    printf("Weather state name: %s\n", weather_state->valuestring);

    cJSON *wind_direction_compass = cJSON_GetObjectItemCaseSensitive(weather_today, "wind_direction_compass");
    if (!cJSON_IsString(wind_direction_compass)) {
        fprintf(stderr, "Invalid wind_direction_compass value");
        exit(EXIT_FAILURE);
    }

    cJSON *wind_direction = cJSON_GetObjectItemCaseSensitive(weather_today, "wind_direction");
    if (!cJSON_IsNumber(wind_direction)) {
        fprintf(stderr, "Invalid wind_direction value");
        exit(EXIT_FAILURE);
    }

    cJSON *wind_speed = cJSON_GetObjectItemCaseSensitive(weather_today, "wind_speed");
    if (!cJSON_IsNumber(wind_speed)) {
        fprintf(stderr, "Invalid wind_speed value");
        exit(EXIT_FAILURE);
    }
    printf(
            "Wind: %s(%f) speed %f\n",
            wind_direction_compass->valuestring,
            wind_direction->valuedouble,
            wind_speed->valuedouble
    );

    cJSON *min_temp = cJSON_GetObjectItemCaseSensitive(weather_today, "min_temp");
    if (!cJSON_IsNumber(min_temp)) {
        fprintf(stderr, "Invalid min_temp value");
        exit(EXIT_FAILURE);
    }

    cJSON *max_temp = cJSON_GetObjectItemCaseSensitive(weather_today, "max_temp");
    if (!cJSON_IsNumber(max_temp)) {
        fprintf(stderr, "Invalid max_temp value");
        exit(EXIT_FAILURE);
    }
    printf("Temperature: %f ... %f\n", min_temp->valuedouble, max_temp->valuedouble);

    cJSON_Delete(weather_info);
}

/*
 * Callback for parsing http response body.
 * Server response maybe set of chunks, that's why response size increase in function body.
 */
size_t response_handler(char *data, size_t size, size_t nmemb, void *resp) {
    struct Response *response = (struct Response *) resp;
    size_t chuck_len = size * nmemb;

    response->body = realloc(response->body, response->size + chuck_len + 1);
    memcpy(&(response->body[response->size]), data, chuck_len);
    response->size += chuck_len;
    response->body[response->size] = 0;

    return chuck_len;

}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        fprintf(stderr, "city is not set\n");
        exit(EXIT_FAILURE);
    }
    char *city = argv[1];

    char *api_url = calloc(URL_LEN, sizeof(char));
    sprintf(api_url, "%ssearch/?query=%s", LOCATION_URL, city);

    CURL *curl = curl_easy_init();
    if (!curl) {
        printf("curl init error");
        exit(EXIT_FAILURE);
    }

    struct Response resp = {.body = NULL, .size = 0};

    curl_easy_setopt(curl, CURLOPT_URL, api_url);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, response_handler);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &resp);
    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        fprintf(stderr, "request failed: %s\n", curl_easy_strerror(res));
        exit(EXIT_FAILURE);
    }

    size_t location_id = extract_woeid(&resp);

    resp.body = NULL;
    resp.size = 0;
    memset(api_url, 0, URL_LEN);

    sprintf(api_url, "%s%zu/", LOCATION_URL, location_id);

    curl_easy_setopt(curl, CURLOPT_URL, api_url);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, response_handler);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &resp);
    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        fprintf(stderr, "request failed: %s\n", curl_easy_strerror(res));
        exit(EXIT_FAILURE);
    }

    print_weather_today(&resp);

    curl_easy_cleanup(curl);
    free(api_url);
    return 0;
}
