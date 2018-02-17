#include <stdio.h>
#include <iostream>
#include <string>
#include <time.h>

std::string formatanswer(std::string file);


int main (){
  std::string file_content ("<h1> Hello World! </h1>");
  std::string answer = formatanswer(file_content);
  std::cout << answer << std::endl;
  return 0;
}

std::string formatanswer(std::string file){
  time_t rawtime;
  struct tm* timeinfo;
  time(&rawtime);
  timeinfo = gmtime(&rawtime);
  char buffer [100];

  //Protocol StatusCode ReasonPhrase
  std::string ans;
  std::string protocol ("HTTP/1.1 "); //FIXME by real protocol variable
  std::string statuscode ("200 "); //FIXME by real statuscode variable
  std::string reasonphrase ("OK\n"); //FIXME by real reasonphrase variable
  ans = protocol + statuscode + reasonphrase;
  
  //Date
  ans.append("Date: ");
  strftime(buffer, 100, "%a, %d %b %G %T GMT", timeinfo);
  std::string tmp (buffer);
  ans.append(tmp);
  ans.append("\n");

  //File Length
  ans.append("Content-length: ");
  std::string length = std::to_string(file.length());
  ans.append(length);
  ans.append("\n\n"); //FIXME to remove /n/n if other header afterward

  //Content
  ans.append(file);

  return ans;
}
