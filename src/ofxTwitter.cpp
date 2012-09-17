/*
 *  ofxTwitter.cpp
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

#include "ofxTwitter.h"
#include "Poco/DateTimeParser.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/URI.h"

void ofxTwitter::setup() {
	
	// listen to http events
    ofAddListener(httpUtils.newResponseEvent, this, &ofxTwitter::newResponse);
    httpUtils.start();
    
	// use dummy data in case we've lost web connection
	data.push_back( Tweet("...") );
	
	delegate = NULL;
	
}


vector<Tweet> ofxTwitter::syncQuery(string query){
	return processResponse(httpUtils.getUrl(query));
}

vector<Tweet> ofxTwitter::syncTwitterQuery(string keywords, Poco::DateTime since, int repliesPerPage, int pageIndex){
	return syncQuery(urlForTwitterQuery(keywords,repliesPerPage,pageIndex));
}


string ofxTwitter::urlForTwitterQuery(string keywords, Poco::DateTime since, int repliesPerPage, int pageIndex){
	string query = "http://search.twitter.com/search.atom?q=";
	Poco::URI::encode(keywords,"#@ ",query);
	query += "&amp;rpp=" + ofToString(repliesPerPage);
	query += "&amp;page=" + ofToString(pageIndex);
	query += "&amp;include_entities=true";
	query += "&amp;result_type=recent";
	if(since>Poco::DateTime(1970,1,1)){
		query += "&amp;since_id" + Poco::DateTimeFormatter::format(since,Poco::DateTimeFormat::RFC1036_FORMAT );
	}

	return query;
}

void ofxTwitter::startTwitterQuery(string keywords, Poco::DateTime since, int repliesPerPage, int pageIndex, int queryIdentifier) {
    
    tweetQueryIdentifier = queryIdentifier;
    
	startQuery(urlForTwitterQuery(keywords,since,repliesPerPage,pageIndex));
}


void ofxTwitter::startQuery(string query) {
	// load data from web
	httpUtils.addUrl(query);    
}


vector<Tweet> ofxTwitter::processResponse(const ofxHttpResponse & response){
	// printf("%s\n", response.responseBody.c_str());
	vector<Tweet> tweets;
	xml.loadFromBuffer(response.responseBody); // parse string
	xml.pushTag("feed"); // change relative root to <feed>

	int nombreDeTweets = xml.getNumTags("entry");
	// iterate through <entry> tags
	for (int i = 0; i < nombreDeTweets; i++) {

		xml.pushTag("entry", i);

		Tweet tweet;
		tweet.id = xml.getValue("id", "", 0).c_str();
		tweet.title = xml.getValue("title", "", 0).c_str();
		int timeZoneDiff;
		Poco::DateTimeParser::parse(xml.getValue("updated", "", 0),tweet.updated,timeZoneDiff);
		Poco::DateTimeParser::parse(xml.getValue("published", "", 0),tweet.published,timeZoneDiff);
		tweet.author.uri  = xml.getValue("author:uri", "", 0).c_str();
		tweet.author.name = xml.getValue("author:name", "", 0).c_str();

		//tweet.language = xml.getValue("twitter:lang", "", 0).c_str();

		for(int j=0; j<xml.getNumTags("link"); j++) {

			string type = xml.getAttribute("link", "type", "", j);
			string rel  = xml.getAttribute("link", "rel", "", j);
			string href = xml.getAttribute("link", "href", "", j);

			if (rel == "image")
				tweet.author.imageUri = href;
			else if (rel == "alternate")
				tweet.link = href;
		}

		tweets.push_back( tweet );

		xml.popTag();

	}
	xml.popTag(); // move out of <feed>
	return tweets;

}

void ofxTwitter::newResponse(ofxHttpResponse &response) {
    data = processResponse(response);
	
	// ok, send back results
	if (delegate) {
		delegate->searchResult(data, tweetQueryIdentifier);
	}
	
}

vector<Tweet> ofxTwitter::getLatestResponse() {
	return data;
}

void ofxTwitter::clear() {
	// listen to http events
    ofRemoveListener(httpUtils.newResponseEvent, this, &ofxTwitter::newResponse);
    httpUtils.stop();
    
//	data.clear();
}

void ofxTwitter::setTimeout(int seconds){
	httpUtils.setTimeoutSeconds(seconds);
}
