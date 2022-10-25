#include "PEView.h"

namespace PEAssistant
{

PEView::PEView(_In_ const std::string& kstrPEFilePath) : PEBase(kstrPEFilePath)
{
    StartMapping(0);
}
PEView::~PEView()
{
    StopMapping(false);
}

void PEView::PrintPEInfo() const throw()
{
    PEBase::PrintPEInfo();
}
DWORD PEView::GetPEHeaderSize(_In_ const unsigned int knHeaderType /*EMHeaderType*/) const throw()
{
    return PEBase::GetPEHeaderSize(knHeaderType);
}

} /*< Namespace PEAssistant */