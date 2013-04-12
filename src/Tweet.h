/*
 *  Tweet.h
 *
 *  Created by Douglas Edric Stanley on 10/12/10
 *  cc-by-sa 2010 www.abstractmachine.net. Some rights reserved.
 *  http://creativecommons.org/licenses/by-sa/3.0/
 *
 *  Adapted from code by Paul Shen
 *  cf. http://in.somniac.me/2010/01/openframeworks-http-requests-xml-parser/
 *
 */

#pragma once

#include "ofMain.h"
#include "Poco/DateTime.h"

struct TweetAuthor {
	string name;
	string username;
	string uri;
	string imageUri;
};

struct Tweet {

	string id;
	string last_id; //to query all tweets since last_id (since_id=last_id)
	string link;
	Poco::DateTime published;
	Poco::DateTime updated;
	string title;
	string language;
	
	TweetAuthor author;
	
	Tweet() { }
	Tweet(string defaultString)	: title(defaultString) { }
	
	string print() {
		string str;
		str += "id:" + id;
		str +=  "\npublished:" + ofToString(published.year())+"/"+ofToString(published.month())+"/"+ofToString(published.day())+" " +ofToString(published.hour())+":"+ofToString(published.minute())+":"+ofToString(published.second());
		str +=  "\nupdated:" + ofToString(updated.year())+"/"+ofToString(updated.month())+"/"+ofToString(updated.day())+" " +ofToString(updated.hour())+":"+ofToString(updated.minute())+":"+ofToString(updated.second());
		str +=  "\nlanguage:" + language;
		str +=  "\nauthor:name:" + author.name;
		str +=  "\nauthor:uri:" + author.uri;
		str +=  "\nauthor:imageUri:" + author.imageUri;
		str +=  "\nlink:" + link;
		str +=  "\ntitle:" + title;
		return str;
	}
	
};
