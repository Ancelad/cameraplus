// -*- qml -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012 Mohammed Sameer <msameer@foolab.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0
import QtSparql 1.0
import CameraPlus 1.0

// QML QtGallery stuff is crap.
// Most of the ideas (and some code) for loading and displaying images are stolen from
// N9QmlPhotoPicker https://github.com/petrumotrescu/N9QmlPhotoPicker

// TODO: Seems losing resources in post capture will not be recovered from.

CameraPage {
        id: page

        controlsVisible: false
        policyMode: CameraResources.PostCapture
        needsPipeline: false

        property Item currentItem: null

        function parseDate(str) {
                var parts = str.split('T');
                var dates = parts[0].split('-');
                var times = parts[1].split(':');
                return new Date(dates[0], dates[1], dates[2], times[0], times[1], times[2]);
        }

        Rectangle {
                color: "black"
                anchors.fill: parent
        }

        PathView {
                id: view
                anchors.fill: parent

                path: Path {
                        startX: - view.width
                        startY: view.height / 2
                        PathLine { x: view.width * 2; y: view.height / 2 }
                }

                flickDeceleration: 999999 // Insanely high value to prevent panning multiple images
                preferredHighlightBegin: 0.5
                preferredHighlightEnd: 0.5
                highlightRangeMode: PathView.StrictlyEnforceRange
                pathItemCount: 3

                model: SparqlListModel {
                        // This is the exact query used by Harmattan gallery.
                        // Gallery prints it as part of its debugging when
                        // passing -output-level debug ;-)
                        query: "SELECT nie:url(?urn) AS ?url nfo:fileName(?urn) AS ?filename ?created nie:mimeType(?urn) AS ?mimetype ( EXISTS { ?urn nao:hasTag nao:predefined-tag-favorite . } ) AS ?favorite nfo:duration(?urn) AS ?duration ?urn \"false\"^^xsd:boolean AS ?isVirtual nfo:fileLastModified(?urn) as ?lastmod nfo:hasRegionOfInterest(?urn) as ?roi tracker:id(?urn) AS ?trackerid WHERE { ?urn rdf:type nfo:Visual ; tracker:available \"true\"^^xsd:boolean . OPTIONAL { ?urn nie:contentCreated ?created } . ?urn nfo:equipment \"urn:equipment:" + deviceInfo.manufacturer + ":" + deviceInfo.model + ":\" . } ORDER BY DESC (?created)"

                        connection: SparqlConnection {
                                id: connection
                                driver: "QTRACKER_DIRECT"
                                onStatusChanged: checkStatus(status)

                                function checkStatus(status) {
                                        if (status == SparqlConnection.Error) {
                                                // TODO: report error
                                                console.log("Error = " + connection.errorString());
                                        }
                                }
                        }
                }

                delegate: PostCaptureItem {
                        width: view.width - 10
                        height: view.height
                }
        }

        ToolBar {
                id: toolBar
                opacity: 0.8
                anchors.bottom: parent.bottom
                tools: ToolBarLayout {
                        id: layout
                        ToolIcon { iconId: "icon-m-toolbar-back"; onClicked: { pageStack.pop(); } }
                }
        }

        ToolBar {
                opacity: toolBar.opacity
                anchors.top: parent.top
                visible: toolBar.visible

                tools: ToolBarLayout {
                        Label {
                                id: filename
                                text: currentItem ? currentItem.fileName : ""
                                anchors.top: parent.top
                                anchors.bottom: parent.bottom
                                anchors.left: parent.left
                                font.bold: true
                                verticalAlignment: Text.AlignVCenter
                        }

                        Label {
                                text: currentItem ? Qt.formatDateTime(parseDate(currentItem.creationDate)) : ""
                                font.bold: true
                                anchors.top: parent.top
                                anchors.bottom: parent.bottom
                                anchors.right: parent.right
                                verticalAlignment: Text.AlignVCenter
                        }
                }
        }
}
