#include <iostream>
#include <fstream>
#include <unordered_set>
#include <algorithm>

using std::cout;            using std::endl;
using std::string;          using std::unordered_set;

unordered_set<string> findWikiLinks(const string& page_html);
string fileToString(std::ifstream& inp);


/*
 * Read the html filestream and put in a sting
 * return the string of txt
*/
string fileToString(std::ifstream& inp) {
    string ret;
    string line;
    while(std::getline(inp, line)) {
        //applies the given function(UnaryOperation unary_op)
        //to a range(line.begin,end) and stores the result in another range, beginning at d_first(line.begin).
        std::transform(line.begin(), line.end(), line.begin(), ::tolower);//set line all lower characters
        ret += line + ""; //用空格分开获取到的每一行
    }
    return ret;
}


/*
 * The aim of this method will be to find all valid Wikipedia links of this
 * form in the input html string
 * and return an unorderd_set containing just the PAGENAME part of the link.
 */
unordered_set<string> findWikiLinks(const string& page_html) {
    unordered_set<string> linkSet;

    //<a href="link_path">link text</a>
    //<a href="/wiki/Marine_mammal">marine mammal</a> native to the

    const string T_PATTERN = "\"/wiki/"; //how to write "in double qute?use \"
    const size_t STEPS = T_PATTERN.size(); //where +15 number come from

    //ilegal characters
    const char HASH = '#';
    const char SEMI_COLON = ':';

    string link;
    auto start = page_html.begin(); //itr_search
    auto end = page_html.end();

    while(start != end) {
        // std::search return Iterator to the beginning of first subsequence [s_first, s_last)
        auto curr = std::search(start, end, T_PATTERN.begin(), T_PATTERN.end());
        start = curr;
        if(curr != end) { //find link start
            auto linkend = std::find(start, end, '"'); //itr_find
            if(linkend != end) { //find line end
                bool isValid = std::all_of(start, linkend,
                                           [ ](char c) ->bool {
                    return (c!=HASH && c!=SEMI_COLON);
                }); // [ ](char c) ->bool { return (c!=HASH && c!=SEMI_COLON);}
                if(isValid) {
                    link = string(start+STEPS,linkend);
                    //for(auto iter=start+STEPS; iter<linkend; iter++) {link += *iter;}
                    linkSet.insert(link);
                    link = "";//attention:defined ouside, remerber to reinit
                }
            }else {
                break;//一旦找不到就结束
            }
            ++linkend;
            start = linkend;
        }else {
            break;// end the loop once don't find one
        }
    }
    return linkSet;
}




int main() {

    unordered_set<string> pagenames;
//    std::ifstream file("simple.txt");
//    string f = fileToString(file);
    string html = "<p> In <a href=\"/wiki/Topology\">topology</a>, the <b>long line</b> (or <b>Alexandroff line</b>) is a <a href=\"/wiki/Topological_space\">topological space</a> somewhat similar to the <a href=\"/wiki/Real_line\">real line</a>, but in a certain way \"longer\". It behaves locally just like the real line, but has different large-scale properties (e.g., it is neither <a href=\"/wiki/Lindel%C3%B6f_space\">Lindelöf</a> nor <a href=\"/wiki/Separable_space\">separable</a>). Therefore, it serves as one of the basic counterexamples of topology <a href=\"http://www.ams.org/mathscinet-getitem?mr=507446\">[1]</a>. Intuitively, the usual real-number line consists of a countable number of line segments [0,1) laid end-to-end, whereas the long line is constructed from an uncountable number of such segments. You can consult <a href=\"/wiki/Special:BookSources/978-1-55608-010-4\">this</a> book for more information. </p>";
    pagenames = findWikiLinks(html);

    //test the result
    for(string pname : pagenames) {
        cout << pname << endl;
    }
    return 0;
    /* Note if your file reading isn't working, please follow the
     * steps posted on the front page of the CS106L website. Essentially
     * go to the projects tab on the panel on the left, and in the run
     * section, uncheck the "Run in terminal" box and re-check it. This
     * should fix things.
     */
}
