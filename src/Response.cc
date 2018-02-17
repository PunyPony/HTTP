#include <string>
#include <ResponseBuilder.hh>

std::string ResponseBuilder::Response::forge_error_response(error_type err)
{
  std::string error_message;
  switch (err)
  {
    case ACCESS_DENIED:
      error_message = "ACCESS_DENIED";
      break;
    case FILE_NOT_FOUND:
      error_message = "FILE_NOT_FOUND";
      break;
    case INTERNAL_ERROR:
      error_message = "INTERNAL_ERROR";
      break;

  }
    return "ERROR " + std::to_string(err) + error_message +"\n";
}

int ResponseBuilder::Response::forge_response()
{
    switch (type_)
    {
    case RQFILE:
        struct fileparams* definedparams = (struct fileparams*)params_;
        std::string path = definedparams->path;
        //fixme: check path bounds
        std::ifstream file(path);
        if (!file.good())
            resonse_ = forge_error_response(FILE_NOT_FOUND); //fixme: check error type(not found, cannot open...)
        else
        {
            std::string content;
            file >> content;

            file.seekg(0, std::ios::end);
            content.reserve(file.tellg());
            file.seekg(0, std::ios::beg);

            content.assign((std::istreambuf_iterator<char>(file)),
                std::istreambuf_iterator<char>());
            response_ = content;
        }
        delete definedparams;
    }
    response_ = forge_error_response(); //or INTERNAL_ERROR for less fun
}

void ResponseBuilder::Response:send_reponse(std::string response)
{
    int res = write(client_sock_, &response[0], response.size()); //should we write <end of file>?
    if (res == -1)
        error("write", 2);
    else
        if ((size_t)res != response.size())
            error("write: wrong size written", 2);
}

void ResponseBuilder::Response::error(std::string msg, int code)
{
    std::cerr << msg << std::endl;
}

