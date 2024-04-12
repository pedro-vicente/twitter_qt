#include "navigation.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QColor>
#include <QString>
#include <QTextStream>

#include <sstream>

///////////////////////////////////////////////////////////////////////////////////////
// MainFeed
///////////////////////////////////////////////////////////////////////////////////////

MainFeed::MainFeed()
{
  populate_twitter();

  layout = new QVBoxLayout(this);

  make_layout_feed("Alice");
}

///////////////////////////////////////////////////////////////////////////////////////
// MainFeed::populate_twitter
///////////////////////////////////////////////////////////////////////////////////////

void MainFeed::populate_twitter()
{
  // create users
  User user1("Alice");
  User user2("Bob");

  // add existing users
  twitter.users.push_back(user1);
  twitter.users.push_back(user2);

  // create a tweet
  Tweet tweet;

  //user take ownership 
  user1.make_tweet(tweet, "Tweet #1");

  // users interact with the tweet
  user2.reply_to_tweet(tweet, "Response #1");

  //like
  user2.like_tweet(tweet);

  //retweet
  user2.retweet(tweet);

  //add to storage
  twitter.post_tweet(tweet);

  // Posting tweets
  for (int idx = 0; idx < 2; idx++)
  {
    std::stringstream s;
    s << "Tweet # " << idx + 2;
    Tweet tweet;
    user1.make_tweet(tweet, s.str());

    //add to storage
    twitter.post_tweet(tweet);
  }
}

///////////////////////////////////////////////////////////////////////////////////////
// MainFeed::make_layout_feed
// display main feed of a user
///////////////////////////////////////////////////////////////////////////////////////

void MainFeed::make_layout_feed(const std::string& username)
{
  User* user = twitter.find_user(username);
  if (user)
  {
    text_edit = new QTextEdit(this);
    text_edit->setFixedSize(300, 60);

    QPushButton* button_tweet = new QPushButton("Tweet", this);
    connect(button_tweet, &QPushButton::clicked, this, &MainFeed::post_tweet);

    //add to layout
    layout->addWidget(text_edit);
    layout->addWidget(button_tweet);

    for (const auto& tweet : user->tweets)
    {
      FeedEntry* entry = new FeedEntry(this, tweet, entry_mode_t::main_feed);

      //add to layout
      layout->addWidget(entry);
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////////
// MainFeed::make_layout_tweet
// display this tweet only and responses 
///////////////////////////////////////////////////////////////////////////////////////

void MainFeed::make_layout_tweet(const Tweet& tweet)
{
  //add "Home" button to main feed
  QPushButton* button_home = new QPushButton("Main feed", this);
  connect(button_home, &QPushButton::clicked, this, &MainFeed::main_feed);

  layout->addWidget(button_home);

  //add tweet and responses 
  FeedEntry* entry = new FeedEntry(this, tweet, entry_mode_t::active);
  layout->addWidget(entry);

  //add responses  
  for (const auto& response : tweet.responses)
  {
    //make a temporary tweet and add the response as content 
    Tweet t;
    t.content = response;
    FeedEntry* entry = new FeedEntry(this, t, entry_mode_t::response);
    layout->addWidget(entry);
  }
}

///////////////////////////////////////////////////////////////////////////////////////
// MainFeed::delete_layout
// delete all child widgets 
///////////////////////////////////////////////////////////////////////////////////////

void MainFeed::delete_layout()
{
  //delete previous feed
  QLayoutItem* child;
  while ((child = layout->takeAt(0)) != 0)
  {
    QWidget* w = child->widget();
    w->hide();
    delete child;
  }
}

///////////////////////////////////////////////////////////////////////////////////////
// MainFeed::switch_page
///////////////////////////////////////////////////////////////////////////////////////

void MainFeed::switch_page(const Tweet& tweet)
{
  //delete previous feed
  delete_layout();

  //make new feed 
  make_layout_tweet(tweet);

  //update layout 
  update();
}

///////////////////////////////////////////////////////////////////////////////////////
// MainFeed::main_feed
///////////////////////////////////////////////////////////////////////////////////////

void MainFeed::main_feed()
{
  //delete previous feed
  delete_layout();

  //make new feed 
  make_layout_feed("Alice");
}

///////////////////////////////////////////////////////////////////////////////////////
// MainFeed::post_tweet
///////////////////////////////////////////////////////////////////////////////////////

void MainFeed::post_tweet()
{
  QString content = text_edit->toPlainText();
  if (!content.isEmpty())
  {
    Tweet tweet;
    text_edit->clear();
  }
}

///////////////////////////////////////////////////////////////////////////////////////
// FeedEntry
///////////////////////////////////////////////////////////////////////////////////////

FeedEntry::FeedEntry(QWidget* p, const Tweet& t, entry_mode_t m) :
  QLabel(p),
  tweet(t),
  entry_mode(m)
{
  setFixedSize(300, 80);

  if (entry_mode == entry_mode_t::main_feed)
  {
    set_color(Qt::darkYellow);
  }
  else if (entry_mode == entry_mode_t::active)
  {
    set_color(Qt::red);
  }
  else if (entry_mode == entry_mode_t::response)
  {
    set_color(Qt::green);
  }

  QString buf;
  QTextStream text(&buf);
  text << "User: " << QString::fromStdString(tweet.username) << "\n";
  text << "Content: " << QString::fromStdString(tweet.content) << "\n";;
  text << "Likes: " << tweet.likes << "\n";;
  text << "Responses: " << tweet.responses.size() << "\n";;
  setText(buf);
}

///////////////////////////////////////////////////////////////////////////////////////
// FeedEntry::mousePressEvent
///////////////////////////////////////////////////////////////////////////////////////

void FeedEntry::mousePressEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton)
  {
    //get parent
    MainFeed* p = (MainFeed*)parentWidget();
    p->switch_page(tweet);
  }
  else
  {
    QLabel::mousePressEvent(event);
  }
}

///////////////////////////////////////////////////////////////////////////////////////
// FeedEntry::set_color
///////////////////////////////////////////////////////////////////////////////////////

void FeedEntry::set_color(const QColor& color)
{
  QPalette pal = QPalette();
  pal.setColor(QPalette::Window, color);
  setAutoFillBackground(true);
  setPalette(pal);
}
