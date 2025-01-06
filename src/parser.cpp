// epubparser.cpp
#include "parser.hpp"
#include "miniz.h" // Include the miniz header
#include <QDebug>
#include <QDir>
#include <QFile>

bool EpubParser::open(const QString &filePath) {
  // Convert the Qt QString to a standard string
  std::string stdPath = filePath.toStdString();

  // Read file into memory
  QFile fp(QString::fromStdString(stdPath));
  if (!fp.open(QIODevice::ReadOnly)) {
    qWarning() << "Failed to open the ePub file:" << filePath;
    return false;
  }
  QByteArray fileData = fp.readAll();
  fp.close();

  // Initialize an mz_zip_archive structure
  mz_zip_archive zip_archive;
  memset(&zip_archive, 0, sizeof(zip_archive));

  // Open the ZIP archive from memory
  if (!mz_zip_reader_init_mem(&zip_archive, fileData.constData(),
                              fileData.size(), 0)) {
    qWarning() << "Failed to initialize ZIP archive with miniz";
    return false;
  }

  // Get the total number of files inside the archive
  mz_uint numFiles = mz_zip_reader_get_num_files(&zip_archive);

  // Traverse each file in the archive
  for (mz_uint i = 0; i < numFiles; i++) {
    mz_zip_archive_file_stat file_stat;
    if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat)) {
      qWarning() << "Failed to get file info for file index" << i;
      continue;
    }

    // Check if the file is an XHTML file (you could also look for ".html")
    QString fileName = QString::fromUtf8(file_stat.m_filename);
    if (fileName.endsWith(".xhtml", Qt::CaseInsensitive)) {
      // Unzip the file content into memory
      size_t uncompressed_size = 0;
      void *p =
          mz_zip_reader_extract_to_heap(&zip_archive, i, &uncompressed_size, 0);
      if (p) {
        m_chapterData.push_back(
            QByteArray(reinterpret_cast<const char *>(p),
                       static_cast<int>(uncompressed_size)));
        m_chapterPathList.push_back(fileName);
        mz_free(p);
      }
    }
  }

  // Always end with cleanup
  mz_zip_reader_end(&zip_archive);

  // At this point, m_chapterData and m_chapterPathList should have the chapter
  // info
  return true;
}

QStringList EpubParser::getChapters() const { return m_chapterPathList; }

QString EpubParser::getChapterContent(int index) const {
  if (index < 0 || index >= static_cast<int>(m_chapterData.size())) {
    return QString();
  }
  // Convert the raw data to a QString (HTML)
  return QString::fromUtf8(m_chapterData[index]);
}
