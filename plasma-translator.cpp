#include "plasma-translator.h"

// Qt
#include <QGraphicsLinearLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QScriptValue>
#include <QScriptEngine>
#include <QScriptValueIterator>
// KDE
#include <KLocale>
#include <KDebug>
#include <KComboBox>

// Plasma
#include <Plasma/Label>
#include <Plasma/LineEdit>
#include <Plasma/TextEdit>
#include <Plasma/ComboBox>

Translator::Translator(QObject *parent, const QVariantList &args) : Plasma::Applet(parent, args),
                                                                    lay(0)
{
  lay = new QGraphicsLinearLayout(this);
  lay->setOrientation(Qt::Vertical);
  setMinimumSize(350,350);
}

Translator::~Translator()
{
 if(!hasFailedToLaunch()){
 // TODO: do some save settings..
 }
}

void Translator::init()
{
 Plasma::Label *m_label_a = new Plasma::Label(this);
 m_label_a->setText(i18n("Put the string to translate"));
                                                             // TODO: set label text
 lay->addItem(m_label_a);                                    // to indicate the source 
 m_ledit = new Plasma::LineEdit(this);                       // language and the destination
 connect(m_ledit, SIGNAL(returnPressed()),                   // language of translation.
            this, SLOT(translation()));
 lay->addItem(m_ledit);        

 // TODO: use a procedure to set languages up
 //       It would be nice to do a setData
 //       for each element to allow easier
 //       language code resolution in place
 //       of a long parsing line as it happens
//        in translation() some lines next.

 source = new Plasma::ComboBox(this);
 source->addItem(i18n("Italian")+" (it)");
 destination = new Plasma::ComboBox(this);
 destination->addItem(i18n("English")+" (en)");
 lay->addItem(source);
 Plasma::Label *m_label_b = new Plasma::Label(this);
 m_label_b->setText(i18n("Translation"));
 lay->addItem(m_label_b);

 m_tedit = new Plasma::TextEdit(this);
 lay->addItem(m_tedit);
 lay->addItem(destination);

}

void Translator::translation()
{
 kDebug()<<"launching";

 QString srcLan,destLan;
 srcLan = source->nativeWidget()->currentText().split(" ", QString::SkipEmptyParts)[1].remove("(").remove(")");
 destLan = destination->nativeWidget()->currentText().split(" ", QString::SkipEmptyParts)[1].remove("(").remove(")");

 QUrl u = QUrl::fromEncoded(QString("http://ajax.googleapis.com/ajax/services/language/translate?v=1.0&q="+m_ledit->text()+"&langpair="+srcLan+"%7C"+destLan).toUtf8());
 QNetworkRequest request(u);
 kDebug()<<request.url();
 QNetworkAccessManager *manager = new QNetworkAccessManager(this);
 reply = manager->get(request);
 connect(reply, SIGNAL(readyRead()), this, SLOT(getTranslation()));
}

void Translator::gotError(int , const QString& err, const QVariant& op)
{
 m_tedit->setText(i18n("Error: ") + err + op.toByteArray());
}


void Translator::getTranslation()
{

  QScriptValue value;
  QScriptEngine engine;
  value = engine.evaluate(reply->readAll());

  m_tedit->setText(value.property("responseData").property("translatedText").toString());
}

void Translator::setLanguages(QComboBox *combo)
{
   combo->addItem(i18n("Arabic"), "ar");
   combo->addItem(i18n("Bulgarian"), "bg");
   combo->addItem(i18n("Catalan"), "ca");
   combo->addItem(i18n("Chinese"), "zh");
   combo->addItem(i18n("Chinese Simplified"), "zh-CN");
   combo->addItem(i18n("Chinese Traditional"), "zh-TW");
}

#include "plasma-translator.moc"
