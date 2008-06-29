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
  setBackgroundHints(TranslucentBackground);
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
 setLanguages(source->nativeWidget());
 destination = new Plasma::ComboBox(this);
 setLanguages(destination->nativeWidget());
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
 srcLan = source->nativeWidget()->itemData(source->nativeWidget()->currentIndex()).toString();
 destLan = destination->nativeWidget()->itemData(destination->nativeWidget()->currentIndex()).toString();

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
   combo->addItem(i18n("Croatian"), "hr");
   combo->addItem(i18n("Czech"), "cs");
   combo->addItem(i18n("Danish"), "da");
   combo->addItem(i18n("Dutch"), "nl");
   combo->addItem(i18n("English"), "en");
   combo->addItem(i18n("Estonian"), "et");
   combo->addItem(i18n("Filipino"), "tl"); 
   combo->addItem(i18n("Finnish"), "fi");
   combo->addItem(i18n("French"), "fr");
   combo->addItem(i18n("German"), "de");
   combo->addItem(i18n("Greek"), "el");
   combo->addItem(i18n("Hebrew"), "iw");
   combo->addItem(i18n("Hindi"), "hi");
   combo->addItem(i18n("Hungarian"), "hu");
   combo->addItem(i18n("Indonesian"), "id");
   combo->addItem(i18n("Italian"), "it");
   combo->addItem(i18n("Japanese"), "ja");
   combo->addItem(i18n("Korean"), "ko");
   combo->addItem(i18n("Latvian"), "lv");
   combo->addItem(i18n("Lithuanian"), "lt");
   combo->addItem(i18n("Norwegian"), "no");
   combo->addItem(i18n("Persian"), "fa");
   combo->addItem(i18n("Polish"), "pl");
   combo->addItem(i18n("Portuguese"), "pt-PT"); 
   combo->addItem(i18n("Romanian"), "ro");
   combo->addItem(i18n("Russian"), "ru");
   combo->addItem(i18n("Serbian"), "sr");
   combo->addItem(i18n("Slovak"), "sk");
   combo->addItem(i18n("Spanish"), "es");
   combo->addItem(i18n("Swedish"), "sv");
   combo->addItem(i18n("Thai"), "th");
   combo->addItem(i18n("Turkish"), "tr");
   combo->addItem(i18n("Ukrainian"), "uk");
   combo->addItem(i18n("Vietnamese"), "vi");
   combo->addItem(i18n("Unknown"), QString());
}

#include "plasma-translator.moc"
