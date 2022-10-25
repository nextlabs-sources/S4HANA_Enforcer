#pragma once
#include "PEBase.h"

namespace PEAssistant
{

class PEView : public PEBase
{
public:
    PEView(_In_ const std::string& kstrPEFilePath);
    ~PEView();

public:
    void PrintPEInfo() const throw();
    // Include NT and Section header size
    DWORD GetPEHeaderSize(_In_ const unsigned int knHeaderType /*EMHeaderType*/) const throw();
};

} /*< Namespace PEAssistant */
