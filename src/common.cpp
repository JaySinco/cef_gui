#include <codecvt>
#include "include/cef_parser.h"
#include "common.h"

std::string GetDataURI(const std::string& data, const std::string& mime_type)
{
    return "data:" + mime_type + ";base64," + CefURIEncode(
        CefBase64Encode(data.data(), data.size()), false).ToString();
}

std::wstring StringToWString(std::string str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.from_bytes(str);
}

std::string WStringToString(std::wstring wstr)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.to_bytes(wstr);
}