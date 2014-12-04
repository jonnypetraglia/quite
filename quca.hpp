#ifndef QUCA_H
#define QUCA_H


#include <QChar>
#include <QString>
#include <QCollator>
#include <algorithm>
#include <iostream>

/* Pulled from http://kdeblog.mageprojects.com/2014/07/06/natural-string-sorting-an-in-depth-analysis/
 * I did not write this, but I changed some names & added a namespace & stuff.
*/

namespace QUnicodeCollationAlgorithm {


    /** Brief summary of each function & (according to the article) when to use each: **/

    /* This uses a compare function that is ripped directly out of KStringHandler from KDE.
     * It is deprecated (or at least, planned to be) by the 2 Collator classes.
     * Use: "When you need a simple natural string compare method"
     */
    template<typename QStringContainer>
    void kHandlerSort(QStringContainer &list, bool reverse=false);

    /* This uses the new QCollator, which is a class designed specifically to compare strings according to locale
     * Use: "I cannot make up any reason anymore to use"
     */
    // NOTE: CANNOT get this to work. QCollator always returns 0 when comparing.
    template<typename QStringContainer>
    void collatorSort(QStringContainer &thing, bool reverse=false);

    /* This is like the above, except it creates a QCollatorKey, which essentially is a way to cache the data needed to compare.
     * In sorting, you end out comparing the same object multiple times, so you can see how this would be beneficial.
     * Use: "If you have more time"
     */
    template<typename QStringContainer>
    void collatorKeyCompare(QStringContainer &thing, bool reverse=false);



    // The compare function from KStringHandler
    int naturalCompare(const QString &_a, const QString &_b, Qt::CaseSensitivity caseSensitivity);



    /**************************************************************************/

    template<typename QContainer>
    void kHandlerSort(QContainer &x, bool reverse)
    {
        if(reverse)
            std::sort(x.begin(), x.end(), [&](const QString& a, const QString& b) { return naturalCompare(a, b, Qt::CaseInsensitive) > 0; });
        else
            std::sort(x.begin(), x.end(), [&](const QString& a, const QString& b) { return naturalCompare(a, b, Qt::CaseInsensitive) < 0; });
    }

    template<typename QContainer>
    void collatorSort(QContainer &x, bool reverse)
    {
        QCollator col;
        col.setNumericMode(true);
        col.setCaseSensitivity(Qt::CaseInsensitive);

        if(reverse)
            std::sort(x.begin(), x.end(), [&](const QString& a, const QString& b) { return col.compare(a, b) > 0; });
        else
            std::sort(x.begin(), x.end(), [&](const QString& a, const QString& b) { return col.compare(a, b) < 0; });
    }

    template<typename QContainer>
    void collatorKeySort(QContainer &thing, bool reverse)
    {
        QCollator col;
        col.setNumericMode(true);
        col.setCaseSensitivity(Qt::CaseInsensitive);

        std::vector<QString> thingCopy;
        thingCopy.reserve(thing.size());
        // This is used for the comparison
        std::vector<QCollatorSortKey> sortKeys;
        // THe indices is what we will actually sort
        std::vector<unsigned int> indices;

        // Create some THINGS
        for(int i = 0; i < thing.size(); i++) {
            thingCopy.push_back(thing[i]);
            sortKeys.emplace_back(col.sortKey(thing[i]));
            indices.push_back(i);
        }

        // Here we are actually sorting the indices
        if(reverse)
            std::sort(indices.begin(), indices.end(), [&](int a, int b) { return sortKeys[b] < sortKeys[a]; });
        else
            std::sort(indices.begin(), indices.end(), [&](int a, int b) { return sortKeys[a] < sortKeys[b]; });

        // Now we gots to actually sort the original array
        for(unsigned int i=0; i<indices.size(); i++)
            thing[i] = thingCopy[indices[i]];
    }

    // Copied from kstringhandler.cpp in kdecore (part of kdelibs).
    int naturalCompare(const QString &_a, const QString &_b, Qt::CaseSensitivity caseSensitivity)
    {
        // This method chops the input a and b into pieces of
        // digits and non-digits (a1.05 becomes a | 1 | . | 05)
        // and compares these pieces of a and b to each other
        // (first with first, second with second, ...).
        //
        // This is based on the natural sort order code code by Martin Pool
        // http://sourcefrog.net/projects/natsort/
        // Martin Pool agreed to license this under LGPL or GPL.

        // FIXME: Using toLower() to implement case insensitive comparison is
        // sub-optimal, but is needed because we compare strings with
        // localeAwareCompare(), which does not know about case sensitivity.
        // A task has been filled for this in Qt Task Tracker with ID 205990.
        // http://trolltech.com/developer/task-tracker/index_html?method=entry&id=205990
        QString a;
        QString b;
        if (caseSensitivity == Qt::CaseSensitive) {
            a = _a;
            b = _b;
        } else {
            a = _a.toLower();
            b = _b.toLower();
        }

        const QChar* currA = a.unicode(); // iterator over a
        const QChar* currB = b.unicode(); // iterator over b

        if (currA == currB) {
            return 0;
        }

        while (!currA->isNull() && !currB->isNull()) {
            const QChar* begSeqA = currA; // beginning of a new character sequence of a
            const QChar* begSeqB = currB;
            if (currA->unicode() == QChar::ObjectReplacementCharacter) {
                return 1;
            }

            if (currB->unicode() == QChar::ObjectReplacementCharacter) {
                return -1;
            }

            if (currA->unicode() == QChar::ReplacementCharacter) {
                return 1;
            }

            if (currB->unicode() == QChar::ReplacementCharacter) {
                return -1;
            }

            // find sequence of characters ending at the first non-character
            while (!currA->isNull() && !currA->isDigit() && !currA->isPunct() && !currA->isSpace()) {
                ++currA;
            }

            while (!currB->isNull() && !currB->isDigit() && !currB->isPunct() && !currB->isSpace()) {
                ++currB;
            }

            // compare these sequences
            const QStringRef& subA(a.midRef(begSeqA - a.unicode(), currA - begSeqA));
            const QStringRef& subB(b.midRef(begSeqB - b.unicode(), currB - begSeqB));
            const int cmp = QStringRef::localeAwareCompare(subA, subB);
            if (cmp != 0) {
                return cmp < 0 ? -1 : +1;
            }

            if (currA->isNull() || currB->isNull()) {
                break;
            }

            // find sequence of characters ending at the first non-character
            while ((currA->isPunct() || currA->isSpace()) && (currB->isPunct() || currB->isSpace())) {
                if (*currA != *currB) {
                    return (*currA < *currB) ? -1 : +1;
                }
                ++currA;
                ++currB;
                if (currA->isNull() || currB->isNull()) {
                    break;
                }
            }

            // now some digits follow...
            if ((*currA == QLatin1Char('0')) || (*currB == QLatin1Char('0'))) {
                // one digit-sequence starts with 0 -> assume we are in a fraction part
                // do left aligned comparison (numbers are considered left aligned)
                while (1) {
                    if (!currA->isDigit() && !currB->isDigit()) {
                        break;
                    } else if (!currA->isDigit()) {
                        return +1;
                    } else if (!currB->isDigit()) {
                        return -1;
                    } else if (*currA < *currB) {
                        return -1;
                    } else if (*currA > *currB) {
                        return + 1;
                    }
                    ++currA;
                    ++currB;
                }
            } else {
                // No digit-sequence starts with 0 -> assume we are looking at some integer
                // do right aligned comparison.
                //
                // The longest run of digits wins. That aside, the greatest
                // value wins, but we can't know that it will until we've scanned
                // both numbers to know that they have the same magnitude.

                bool isFirstRun = true;
                int weight = 0;
                while (1) {
                    if (!currA->isDigit() && !currB->isDigit()) {
                        if (weight != 0) {
                            return weight;
                        }
                        break;
                    } else if (!currA->isDigit()) {
                        if (isFirstRun) {
                            return *currA < *currB ? -1 : +1;
                        } else {
                            return -1;
                        }
                    } else if (!currB->isDigit()) {
                        if (isFirstRun) {
                            return *currA < *currB ? -1 : +1;
                        } else {
                            return +1;
                        }
                    } else if ((*currA < *currB) && (weight == 0)) {
                        weight = -1;
                    } else if ((*currA > *currB) && (weight == 0)) {
                        weight = + 1;
                    }
                    ++currA;
                    ++currB;
                    isFirstRun = false;
                }
            }
        }

        if (currA->isNull() && currB->isNull()) {
            return 0;
        }

        return currA->isNull() ? -1 : + 1;
    }

}

#endif
