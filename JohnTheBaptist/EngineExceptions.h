#pragma once

#include <string>

class EngineException
{
private:
    std::string ExceptionTitle;
    std::string ExceptionDetails;

public:
    EngineException(std::string Title, std::string Details)
    {
        ExceptionTitle = Title;
        ExceptionDetails = Details;
    }

    std::string GetTitle()
    {
        return ExceptionTitle;
    }

    std::string GetDetails()
    {
        return ExceptionDetails;
    }
};