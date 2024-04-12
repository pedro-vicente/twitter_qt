#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QtWidgets>
#include <QWidget>
#include <QLabel>
#include "twitter.h"

QT_BEGIN_NAMESPACE
class QColor;
class QVBoxLayout;
QT_END_NAMESPACE

///////////////////////////////////////////////////////////////////////////////////////
// MainFeed
// main feed shows no responses 
///////////////////////////////////////////////////////////////////////////////////////

class MainFeed : public QWidget
{
  Q_OBJECT
public:
  MainFeed();
  void switch_page(const Tweet& tweet);

  Twitter twitter;
  void populate_twitter();
  void make_layout_feed(const std::string& username);
  void make_layout_tweet(const Tweet& tweet);
  void delete_layout();

  QVBoxLayout* layout;
  QTextEdit* text_edit;

  void post_tweet();
  void main_feed();
};

///////////////////////////////////////////////////////////////////////////////////////
// FeedEntry
//a feed entry displays a "tweet";
//it has text, images, media, a list of responses, list of likes, retweets
//a feed entry has 2 states :
//1) regular mode : a regular entry in the feed, an element in the feed list
//2) active mode : after mouse click, covers all feed area, displays responses
//3) a stack is made to return to original post; this feed is added to stack,
//4) add a "<- Home" at top, to return to original post(regular mode)
//5) click on a response, puts that feed entry in active mode, and adds it to the stack
///////////////////////////////////////////////////////////////////////////////////////

enum class entry_mode_t
{
  main_feed, //entry in feed list
  active, // display responses, hide main feed
  response //entry is a response 
};

class FeedEntry : public QLabel
{
  Q_OBJECT
public:
  FeedEntry(QWidget* parent, const Tweet& tweet, entry_mode_t entry_mode);
  void set_color(const QColor& color);
  Tweet tweet;

protected:
  entry_mode_t entry_mode;
  void mousePressEvent(QMouseEvent* event) override;
};

#endif
