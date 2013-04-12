/*
 *  ofxTwitter.h
 *
 *  Created by Douglas Edric Stanley on 10/12/10
 *  cc-by-sa 2010 www.abstractmachine.net. Some rights reserved.
 *  http://creativecommons.org/licenses/by-sa/3.0/
 *
 *  Adapted from code by Paul Shen
 *  cf. http://in.somniac.me/2010/01/openframeworks-http-requests-xml-parser/
 *
 *  Edited by Andrew Vergara on 05/04/12
 *  Updated addon to fit most recent version of ofxHttpUtils addon. https://github.com/arturoc/ofxHttpUtils
 *
 * 
 */

#pragma once

#include "ofMain.h"

#include "ofxHttpUtils.h"
#include "ofxXmlSettings.h"

#include "TwitterDelegate.h"
#include "Tweet.h"

class ofxTwitter {
public:
	
	void setup();
	void setSearchDelegate(TwitterDelegate *_delegate) { delegate = _delegate; }
	
	vector<Tweet> syncQuery(string query);
	vector<Tweet> syncTwitterQuery(string keywords, Poco::DateTime since=Poco::DateTime(1970,1,1), int repliesPerPage=10, int pageIndex=1);
	vector<Tweet> syncTwitterQuery(string keywords, string since_id, int repliesPerPage=10, int pageIndex=1);


	void startQuery(string query);
	void startTwitterQuery(string keywords, Poco::DateTime since=Poco::DateTime(1970,1,1), int repliesPerPage=10, int pageIndex=1, int queryIdentifier=0);
	void startTwitterQuery(string keywords, string since_id = "", int repliesPerPage=10, int pageIndex=1, int queryIdentifier=0);

	
	string urlForTwitterQuery(string keywords, Poco::DateTime since=Poco::DateTime(1970,1,1), int repliesPerPage=10, int pageIndex=1);
	string urlForTwitterQuery(string keywords, string sind_id, int repliesPerPage=10, int pageIndex=1);

	vector<Tweet> getLatestResponse();
	void newResponse(ofxHttpResponse &response);
    
    void clear();
    void setTimeout(int seconds);

	ofxHttpUtils httpUtils;
    
    int tweetQueryIdentifier;
    
private:
    vector<Tweet> processResponse(const ofxHttpResponse & response);
	TwitterDelegate *delegate;
	
	ofxXmlSettings xml;
	vector<Tweet> data;
	
};
