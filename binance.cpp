#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <iostream>
#include <string>

using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::json;

int main() {
    // URL de la API pública de Binance para obtener información de símbolos.
    const std::string binanceBaseUrl = "https://api.binance.com/api/v3/exchangeInfo";

    // Crear el cliente HTTP.
    http_client client(binanceBaseUrl);

    // Realizar la solicitud GET para obtener la información de los símbolos.
    client.request(methods::GET).then([](http_response response) {
        // Verificar que la solicitud fue exitosa (código 200).
        if (response.status_code() == status_codes::OK) {
            // Leer el contenido de la respuesta como un objeto JSON.
            return response.extract_json();
        } else {
            // Mostrar un mensaje de error en caso de que la solicitud falle.
            std::cerr << "Error al obtener los datos: " << response.status_code() << std::endl;
            return pplx::task_from_result(json::value());
        }
    }).then([](json::value jsonData) {
        // Procesar el objeto JSON para obtener la lista de símbolos.
        if (jsonData.is_object() && jsonData.has_field(U("symbols"))) {
            const json::array& symbolsArray = jsonData[U("symbols")].as_array();
            for (const auto& symbol : symbolsArray) {
                std::wcout << symbol[U("symbol")].as_string() << std::endl;
            }
        } else {
            std::cerr << "Respuesta inválida del servidor." << std::endl;
        }
    }).wait();

    return 0;
}
