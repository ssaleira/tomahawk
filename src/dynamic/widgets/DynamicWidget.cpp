/****************************************************************************************
 * Copyright (c) 2010 Leo Franchi <lfranchi@kde.org>                                    *
 *                                                                                      *
 * This program is free software; you can redistribute it and/or modify it under        *
 * the terms of the GNU General Public License as published by the Free Software        *
 * Foundation; either version 2 of the License, or (at your option) any later           *
 * version.                                                                             *
 *                                                                                      *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY      *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.             *
 *                                                                                      *
 * You should have received a copy of the GNU General Public License along with         *
 * this program.  If not, see <http://www.gnu.org/licenses/>.                           *
 ****************************************************************************************/

#include "DynamicWidget.h"

#include <QVBoxLayout>
#include <QLabel>

#include "DynamicControlList.h"
#include "playlistview.h"
#include "playlistmodel.h"
#include "trackproxymodel.h"
#include "dynamic/GeneratorInterface.h"
#include <QComboBox>
#include "dynamic/GeneratorFactory.h"
#include <QPushButton>

using namespace Tomahawk;

DynamicWidget::DynamicWidget( const Tomahawk::dynplaylist_ptr& playlist, QWidget* parent )
    : QWidget(parent)
    , m_playlist( playlist )
    , m_layout( new QVBoxLayout )
    , m_headerText( 0 )
    , m_headerLayout( 0 )
    , m_modeCombo( 0 )
    , m_generatorCombo( 0 )
    , m_logo( 0 )
    , m_generateButton( 0 )
    , m_controls( 0 )
    , m_splitter( 0 )
    , m_view( 0 )
    , m_model()
{   
    m_headerLayout = new QHBoxLayout;
    m_headerText = new QLabel( "Dynamic Playlist Type:", this );
    m_headerLayout->addWidget( m_headerText );
    m_modeCombo = new QComboBox( this );
    m_modeCombo->addItem( "Static", 0 );
    m_modeCombo->addItem( "On Demand", 1 );
    m_headerLayout->addWidget( m_modeCombo );
    m_generatorCombo = new QComboBox( this );
    foreach( const QString& type, GeneratorFactory::types() )
        m_generatorCombo->addItem( type );
    m_headerLayout->addWidget( m_generatorCombo );
    
    m_headerLayout->addSpacing( 1 );
    
    m_generateButton = new QPushButton( this );
    m_generateButton->hide();
    if( playlist->mode() == Static ) {
        m_generateButton->show();
        m_headerLayout->addWidget( m_generateButton );
    }
    
    m_layout->addLayout( m_headerLayout );
    
    m_splitter = new AnimatedSplitter( this );
    m_splitter->setOrientation( Qt::Vertical );
    m_splitter->setChildrenCollapsible( false );
    
    m_layout->addWidget( m_splitter );
    m_controls = new DynamicControlList( m_splitter );
    m_model = new PlaylistModel( this );
    m_view = new PlaylistView( this );
    m_view->setModel( m_model );
    
    m_splitter->addWidget( m_controls );
    m_splitter->addWidget( m_view );
    m_splitter->setGreedyWidget( 1 );
    
    m_splitter->show( 0, false );
    
    if( !m_playlist.isNull() ) {
        m_controls->setControls( m_playlist->generator()->controls() );
        
        m_model->loadPlaylist( m_playlist );
    }
    
    setLayout( m_layout );
}

DynamicWidget::~DynamicWidget()
{

}

void 
DynamicWidget::setPlaylist(const Tomahawk::dynplaylist_ptr& playlist)
{
    
}

PlaylistInterface* 
DynamicWidget::playlistInterface() const
{
    return m_view->proxyModel();
}
