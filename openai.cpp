#include "base64.h"
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <QCoreApplication>

#include "vendor/json.hpp"
#include "openai.h"
#include "vendor/httplib.h"
using json = nlohmann::json;


std::string describeImage(fs::path p) {
    std::string base_64_string = base64EncodeFile(p);

    std::string API_KEY = qgetenv("OPENAI_API_KEY").toStdString();

    httplib::Client cli("https://api.openai.com");

    httplib::Headers headers = {
        {"Authorization", "Bearer " + API_KEY},
        {"Content-Type", "application/json"}
    };

    json request_body = {
        {"model", "gpt-4o-mini"},

        {"messages", {
                         {
                             {"role", "user"},
                             {"content", json::array({
                                             {
                                                 {"type", "text"},
                                                 {"text", "List out the main identifiable contents of what you see in this image. Do not use punctuation. Only return a list of what you see."}
                                             },
                                             {
                                                 {"type", "image_url"},
                                                 {"image_url", {
                                                                   {"url", "data:image/jpeg;base64," + base_64_string}
                                                               }}
                                             }
                                         })}
                         }
                     }},
        {"max_tokens", 300}
    };

    httplib::Result res = cli.Post("/v1/chat/completions", headers, request_body.dump(), "application/json");
    json response_json = json::parse(res->body);
    std::string description = response_json["choices"][0]["message"]["content"];
    return description;

}

std::vector<float> getEmbedding(std::string description){
    std::string API_KEY = qgetenv("OPENAI_API_KEY").toStdString();


    httplib::Client cli("https://api.openai.com");

    httplib::Headers headers = {
        {"Authorization", "Bearer " + API_KEY},
        {"Content-Type", "application/json"}
    };

    json request_body = {
                         {"input", description},
                         {"model", "text-embedding-3-small"},
                         };


    httplib::Result res = cli.Post("/v1/embeddings", headers, request_body.dump(), "application/json");
    json response_json = json::parse(res->body);

    return response_json["data"][0]["embedding"];
}


