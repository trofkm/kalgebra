/*************************************************************************************
 *  Copyright (C) 2010 by Aleix Pol <aleixpol@kde.org>                               *
 *                                                                                   *
 *  This program is free software; you can redistribute it and/or                    *
 *  modify it under the terms of the GNU General Public License                      *
 *  as published by the Free Software Foundation; either version 2                   *
 *  of the License, or (at your option) any later version.                           *
 *                                                                                   *
 *  This program is distributed in the hope that it will be useful,                  *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of                   *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                    *
 *  GNU General Public License for more details.                                     *
 *                                                                                   *
 *  You should have received a copy of the GNU General Public License                *
 *  along with this program; if not, write to the Free Software                      *
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA   *
 *************************************************************************************/

#include <QApplication>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <analitzagui/operatorsmodel.h>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    OperatorsModel m;

    QFile f(app.arguments().constLast());
    bool fileopened = f.open(QFile::WriteOnly);
    Q_ASSERT(fileopened);

    QTextStream str(&f);

    str << "<!-- this file is autogenerated, if any change has to be done, please "
           "fix kdeedu/kalgebra/utils/main.cpp and its dependencies. "
           "Thank you, Aleix Pol -->";
    str << "<chapter id='commands'>\n"
           "<title>Commands supported by KAlgebra</title>\n";

    int rows = m.rowCount(), cols = m.columnCount();
    QStringList colHeaders;
    for (int i = 0; i < cols; i++)
        colHeaders += m.headerData(i, Qt::Horizontal).toString();

    for (int i = 0; i < rows; i++) {
        QString id = m.index(i, 0).data().toString();
        str << "\t<sect1 id='" << id << "'><title>" << id << "</title><itemizedlist>\n";
        for (int c = 0; c < cols; c++)
            str << QStringLiteral("\t\t<listitem><para>%1: %2</para></listitem>").arg(colHeaders[c], m.index(i, c).data().toString().toHtmlEscaped()) << '\n';

        str << "\t</itemizedlist></sect1>\n";
    }
    str << "</chapter>\n";

    return 0;
}
