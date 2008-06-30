// Translator Plasmoid
// Copyright (c) 2008 By Alessandro Diaferia <alediaferia@gmail.com>

#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <Plasma/Applet>

namespace Plasma{
 class Label;
 class TextEdit;
 class ComboBox;
}

class QGraphicsLinearLayout;
class QNetworkReply;
class QComboBox;

class Translator : public Plasma::Applet
{
 Q_OBJECT
   public: 
    Translator(QObject*parent, const QVariantList &args);
    ~Translator();

   void init();

    private:
      QGraphicsLinearLayout *lay;
      Plasma::TextEdit *m_ledit;
      Plasma::TextEdit *m_tedit;
      QNetworkReply *reply;
      Plasma::ComboBox *source;
      Plasma::ComboBox *destination;

    protected slots:
      virtual void translation();
      virtual void gotError(int , const QString&, const QVariant&);
      virtual void getTranslation();
    protected:
              void setLanguages(QComboBox*);
              void setCurrentLanguage();
      
};

// Since we use a KTextEdit even for the source string
// seems useful to have Enter button's default behavior
// to launch the translation instead of make a new line 
// in the editor. To achieve this we use an event filter
// which allows new line only if Enter is pressed with 
// Shift modifier. Otherwise Enter starts translation.

class enterButtonFilter : public QObject
{
  Q_OBJECT
   public:
    enterButtonFilter(){}
   
   protected:
    bool eventFilter(QObject *obj, QEvent *ev);

   signals:
    void enterPressed();
};
K_EXPORT_PLASMA_APPLET(translator, Translator)
#endif 
