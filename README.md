# twitter_qt
Twitter like interface with Qt  :cyclone:



## Tweet
a "tweet" is a message on the network  
it has text, images, media, a list of responses, list of likes, retweets

```cpp
class Tweet
{
public:
  Tweet() : likes(0) {}
  std::string username;
  std::string content;
  size_t likes;
  std::vector<std::string> responses;
};
```

## User
a user has a name and a list of messages posted   
he/she can reply, like, retweet

```cpp
class User
{
public:
  User(std::string u) : username(u) {}
  std::string username;
  std::vector<Tweet> tweets;

  //assign ownership, but do not store (will be done by Twitter)
  void make_tweet(Tweet& tweet, const std::string& content);
  void reply_to_tweet(Tweet& tweet, std::string response);
  void like_tweet(Tweet& tweet);
  void retweet(Tweet& tweet);
};
```

## Twitter
storage for users;  
context is application memory only, i.e loaded messages (from cache, network)  
actions are:   
1) post a message by a current user
2) add a user

```cpp
class Twitter
{
public:
  Twitter() {}
  std::vector<User> users;

  User* find_user(const std::string& username);
  void post_tweet(const Tweet& tweet);
};
```