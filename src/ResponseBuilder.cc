#include <ResponseBuilder.hh>
#include <iostream>
#include <string>

ResponseBuilder::ResponseBuilder()
{}

/*
std::string get_request(int client_sock)
{
    size_t blocksize = 500;
    size_t totalsize = 0;
    std::string request;
    int res = 0;
    while(res != -1)
    {
	request.reserve(totalsize + blocksize);
	request.resize(totalsize + blocksize);
	res = read(client_sock, &request[totalsize], blocksize);
	totalsize += res;
	if(is_request_full(request)) //warning: request not truncated to real size at this point (->pass size, resize may cost much)
	{
	    totalsize -= 1;
	    break;
	}
    }
    request.resize(totalsize + 1);
    return request;
}

request_type get_request_type(std::string request, void*& params)
{
    request = request;
    //fixme: mutiple request types possible?
    params = new struct fileparams;
    ((struct fileparams*)params)->path = "testfile";
    return RQFILE;
}

std::string forge_response(request_type rqtype, void* params)
{
    std::string result;
    switch(rqtype)
    {
    case RQFILE:
	struct fileparams* definedparams = (struct fileparams*)params;
	std::string path = definedparams->path;
	//fixme: check path bounds
	std::ifstream file(path);
	if (!file.good())
	    result = forge_error_response(FILE_NOT_FOUND); //fixme: check error type(not found, cannot open...)
	else
	{
	    std::string content;
	    file >> content;
    
	    file.seekg(0, std::ios::end);   
	    content.reserve(file.tellg());
	    file.seekg(0, std::ios::beg);
    
	    content.assign((std::istreambuf_iterator<char>(file)),
			   std::istreambuf_iterator<char>());
	    result = content;
	}
	delete definedparams;
	return result;
    }
    return forge_error_response(NIQUE_TA_MERE); //or INTERNAL_ERROR for less fun
}*/