#ifndef LOGSTREAM_H
#define LOGSTREAM_H

#include <streambuf>
#include <iostream>
#include <QString>
#include <QTextEdit>
#include <QMetaObject>
#include <QObject>

class LogEmitter : public QObject
{
    Q_OBJECT

public:
    explicit LogEmitter(QObject *parent = nullptr) : QObject(parent) {}

signals:
    void newLogMessage(const QString& msg);
};

class LogStream : public std::basic_streambuf<char>
{
public:
    LogStream(LogEmitter* emitter) : m_emitter(emitter) {
        old_buf = std::cout.rdbuf(this); // redirect std::cout
    }

    ~LogStream() {
        std::cout.rdbuf(old_buf); // restore original std::cout
    }

protected:
    virtual int_type overflow(int_type v) override {
        if (v == '\n') {
            flushToGui();
        } else {
            buffer += static_cast<char>(v);
        }
        return v;
    }

    virtual std::streamsize xsputn(const char *p, std::streamsize n) override {
        for (std::streamsize i = 0; i < n; ++i)
            overflow(p[i]);
        return n;
    }

private:
    std::string buffer;
    LogEmitter* m_emitter;
    std::streambuf* old_buf;

    void flushToGui() {
        if (!buffer.empty()) {
            QString message = QString::fromStdString(buffer);
            // Emit log to GUI thread safely using Qt queued connection
            QMetaObject::invokeMethod(m_emitter, [this, message]() {
                emit m_emitter->newLogMessage(message);
            }, Qt::QueuedConnection);
            buffer.clear();
        }
    }
};

#endif // LOGSTREAM_H
