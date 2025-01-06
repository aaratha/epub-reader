#pragma once

#include <QByteArray>
#include <QString>
#include <QStringList>
#include <vector>

class EpubParser {
public:
  EpubParser() = default;
  bool open(const QString &filePath);
  QStringList getChapters() const;
  QString getChapterContent(int index) const;

private:
  QStringList m_chapterPathList; // Stores the file paths or names of each
                                 // chapter found in the archive
  std::vector<QByteArray> m_chapterData; // Raw XHTML data for each chapter
};
