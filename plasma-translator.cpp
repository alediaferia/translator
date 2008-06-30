#include "plasma-translator.h"

// Qt
#include <QGraphicsLinearLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QScriptValue>
#include <QScriptEngine>
#include <QScriptValueIterator>
#include <QToolButton>
#include <QGraphicsProxyWidget>
#include <QGraphicsLinearLayout>

// KDE
#include <KLocale>
#include <KDebug>
#include <KComboBox>
#include <KIcon>
#include <KTextEdit>

// Plasma
#include <Plasma/Label>
#include <Plasma/TextEdit>
#include <Plasma/ComboBox>
#include <Plasma/Icon>

Translator::Translator(QObject *parent, const QVariantList &args) : Plasma::Applet(parent, args),
                                                                    lay(0)
{
  lay = new QGraphicsLinearLayout(this);
  lay->setOrientation(Qt::Vertical);
  setBackgroundHints(DefaultBackground);
  setMinimumSize(350,300);
}

Translator::~Translator()
{
 if(!hasFailedToLaunch()){
 // TODO: do some save settings..
 }
}

void Translator::init()
{
 
 //TODO: add Google logo
                                   
 m_ledit = new Plasma::TextEdit(this);                       
 m_ledit->nativeWidget()->setAcceptRichText(false);
 m_ledit->nativeWidget()->setAutoFormatting(QTextEdit::AutoNone);

 Plasma::Icon *go = new Plasma::Icon(this);
 go->setIcon(KIcon("go-jump-locationbar"));
 go->setDrawBackground(true);

 QGraphicsLinearLayout *h_lay = new QGraphicsLinearLayout();
 h_lay->setOrientation(Qt::Horizontal);
 h_lay->addItem(m_ledit);
 h_lay->addItem(go);

 connect(m_ledit, SIGNAL(returnPressed()), this, SLOT(translation()));
 connect(go, SIGNAL(clicked()), this, SLOT(translation()));

 QGraphicsLinearLayout *lay_s = new QGraphicsLinearLayout();
 lay_s->setOrientation(Qt::Horizontal);

 Plasma::Label *m_label_a = new Plasma::Label(this);
 m_label_a->setText(i18n("From:"));
 lay_s->addItem(m_label_a);

 source = new Plasma::ComboBox(this);
 setLanguages(source->nativeWidget());
 source->nativeWidget()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

 lay_s->addItem(source);
 lay->addItem(lay_s); // Here we add the source ComboBox
 lay->addItem(h_lay); // Here we add the source TextEdit

 QGraphicsLinearLayout *lay_t = new QGraphicsLinearLayout();
 lay_t->setOrientation(Qt::Horizontal);
 
 Plasma::Label *m_label_b = new Plasma::Label(this);
 m_label_b->setText(i18n("To:"));
 lay_t->addItem(m_label_b);

 destination = new Plasma::ComboBox(this);
 setLanguages(destination->nativeWidget());
 destination->nativeWidget()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
 lay_t->addItem(destination);

 

 m_tedit = new Plasma::TextEdit(this);
 lay->addItem(lay_t);  //here we add the destination ComboBox
 lay->addItem(m_tedit);  //here we add the destination textEdit

  setCurrentLanguage();
}

void Translator::translation()
{

 QString srcLan,destLan;
 srcLan = source->nativeWidget()->itemData(source->nativeWidget()->currentIndex()).toString();
 destLan = destination->nativeWidget()->itemData(destination->nativeWidget()->currentIndex()).toString();


 QUrl u = QUrl::fromEncoded(QString("http://ajax.googleapis.com/ajax/services/language/translate?v=1.0&q="+
                            m_ledit->nativeWidget()->toPlainText()+"&langpair="+srcLan+"%7C"+destLan).toUtf8());
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

void Translator::setCurrentLanguage()
{
 for(int i=0; i<destination->nativeWidget()->count(); i++){
  kDebug() << destination->nativeWidget()->itemData(i).toString();
  kDebug() << KLocale::defaultLanguage();
  if(KLocale::defaultLanguage().contains( destination->nativeWidget()->itemData(i).toString(), Qt::CaseInsensitive )
     && !destination->nativeWidget()->itemData(i).toString().isEmpty() )

    destination->nativeWidget()->setCurrentIndex(i);
    //break;
 }
}

#include "plasma-translator.moc"
