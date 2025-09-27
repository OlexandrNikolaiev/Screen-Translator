#ifndef ICLIPBOARDSERVICE_H
#define ICLIPBOARDSERVICE_H

#pragma once
#include <QPixmap>

class IClipboardService {
public:
    virtual ~IClipboardService() = default;

    virtual void backup() = 0;
    virtual void setTemporaryPixmap(const QPixmap& pixmap) = 0;
    virtual void restore() = 0;
};


#endif // ICLIPBOARDSERVICE_H
