
#include <iostream>

#include <unordered_set>
#include <vector>
#include <queue>
#include <unordered_map>

#include <algorithm>
#include "wikiscraper.h"


using std::cout;            using std::endl;
using std::string;          using std::vector;
using std::priority_queue;  using std::unordered_map;
using std::unordered_set;

/* 
 * Get the numbers of links in common between set of links on page and the set of links on end_pag
*/
// int comslink(string page, unordered_set<string>& target_set, WikiScraper& scraper) {
//     int count = 0;
//     unordered_set<string> page_set = scraper.getLinkSet(page);
//     auto start = target_set.begin();
//     auto end = target_set.end();
//     while(start != end) {
//         auto curr = std::search(start, end,
//                                 page_set.begin(), page_set.end());
//         if(curr == end) break;
//         ++ count;
//         ++ curr;
//         start = curr;//start = curr + 1; wrong?
//     }
//     return count;
// }

/*
 * This is the function you will be implementing. It takes
 * two string representing the names of a start_page and
 * end_page and is supposed to return a ladder, represented
 * as a vector<string>, of links that can be followed from
 * start_page to get to the end_page.
 *
 * For the purposes of this algorithm, the "name" of a Wikipedia
 * page is what shows at the end of the URL when you visit that page
 * in your web browser. For ex. the name of the Stanford University
 * Wikipedia page is "Stanford_University" since the URL that shows
 * in your browser when you visit this page is:
 *
 *       https://en.wikipedia.org/wiki/Stanford_University
 */
vector<string> findWikiLadder(const string& start_page, const string& end_page) {
    //creates WikiScraper object
    WikiScraper scraper;
    //get the set of links on page specified by end_page
    auto target_set = scraper.getLinkSet(end_page);
    //map store the priority of pages:page->number of common links with end_page
    //why use this? cuase diffeent ladder may have same page at end,reduce redundent compute
    unordered_map<string, size_t> p_map;

    //comparison function for priority_queue //attention:two ladders are params
    // [ &scraper, &target_set, &p_map ] are inital capure list not params
    auto cmpFn = [ &scraper, &target_set, &p_map ](const vector<string>& ladder1, const vector<string>& ladder2) ->bool {

        string page1 = ladder1.at(ladder1.size() -1);//ladder is const,not pop_back
        string page2 = ladder2.at(ladder2.size() -1);
        unordered_set<string> links;
        size_t priority1;
        size_t priority2;

        if(p_map.count(page1)>0) {
            //key page1 is already in the priority map 
            priority1 = p_map[page1];
        }else { 
            //compute its priority
            links = scraper.getLinkSet(page1);
            //count the appear time(which common link > 1) of the links(string s) in page1
            priority1 = std::count_if(links.begin(), links.end(), [& target_set](const string& s) ->bool {
                        return (target_set.count(s) > 0);
                    });
            //add in the map 
            p_map[page1] = priority1;
        }

        //same is page2 
        if(p_map.count(page2)) {
            priority2 = p_map[page2];
        }else {
            links = scraper.getLinkSet(page2);
            priority2 = std::count_if(links.begin(), links.end(), [& target_set](string s) ->bool {
                        return (target_set.count(s) > 0);
                    });
            p_map[page2] = priority2;
        } 
        //bool of compare priority in queue
        //front have high priority??
        return priority1 < priority2;
    };


    //creates a priority_queue of ladders names ladderQueue
    std::priority_queue< vector<string>, vector<vector<string>>, decltype(cmpFn) >    ladderQueue(cmpFn);

    //defined outside the loop
    unordered_set<string> visited; //means the state/page has already be explored
    unordered_set<string> links;
    vector<string> ladder;
    vector<string> newladder;

    //add a ladder containiing {start_page} to the queue
    ladder.push_back(start_page);
    ladderQueue.push(ladder);
    //how to print a queue without change it ?
    vector<string> test;
    test = ladderQueue.top();
    for(string s : test) cout << s ;
    cout << "this is begining of each loop" << endl;

    string page;// represent the ladder's process/state
    while(!ladderQueue.empty()) {
        //dequeue the highest priority  partial-ladder from the fornt of the queue
        ladder = ladderQueue.top();
        ladderQueue.pop();
        //get the set of links of the current page(the page at the end of the just dequeued ladder
        page = ladder.at(ladder.size() -1);
        links = scraper.getLinkSet(page);
        //if end_page in set,find the ladder!
        if(links.count(end_page)) { //can be included in the for loop below
            ladder.push_back(end_page);
            return ladder;
        }
        //neighbor page
        string link;
        for (string link : links) { //same as//not include itr=links.end()???: for(auto itr = links.begin(); itr!=links.end(); ++itr)
            //not already been visited
            // if is visted, means some ladder have already explore this state

            // auto isVisited = [ ](const string page, vector<string>& ladder) ->bool{
            //     for(string p : ladder) {
            //         if(p == page) return true;
            //     }
            //     return false;
            // };
            if(visited.count(link) == 0) {
                //create copy of the current partital ladder
                std::copy(ladder.begin(), ladder.end(), std::back_inserter(newladder));
                //or just newladder = ladder;
                //put the neighbor page string on top of the copied ladder
                newladder.push_back(link);
                //add the copied ladder to the queue
                ladderQueue.push(newladder);
            }
        }
        visited.insert(page);//this page is explored
    }
    //If while loop exits/finish without return, no ladder was found so return an empty vector<string>
    ladder.clear();
    return ladder;
}



int main() {
    auto ladder = findWikiLadder("Fruit", "Strawberry");
    cout << endl;

    if(ladder.empty()) {
        cout << "No ladder found!" << endl;
    } else {
        cout << "Ladder found:" << endl;
        cout << "\t";

        //print ladder
        for(string page: ladder) {
            cout << page << endl;
        }
    }

    return 0;
}




