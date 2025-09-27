#ifndef CLIPBOARDMANAGER_H
#define CLIPBOARDMANAGER_H

#pragma once
#include "IClipboardService.h"
#include <QMimeData>
#include <QPixmap>

class ClipboardManager : public IClipboardService {
public:
    static ClipboardManager& instance();

    void backup() override;
    void setTemporaryPixmap(const QPixmap& pixmap) override;
    void restore() override;

private:
    ClipboardManager();
    ~ClipboardManager();

    ClipboardManager(const ClipboardManager&) = delete;
    ClipboardManager& operator=(const ClipboardManager&) = delete;

    void clearBackup();

    QMimeData* _backup = nullptr;
};


#endif // CLIPBOARDMANAGER_H
