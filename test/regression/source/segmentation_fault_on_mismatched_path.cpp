/*
 * Copyright (c) 2013, 2014, 2015 Corvusoft
 *
 * bug tracker issue #4
 */

//System Includes
#include <memory>
#include <functional>

//Project Includes
#include <restbed>
#include "helpers/http.h"

//External Includes
#include <gtest/gtest.h>

//System Namespaces
using std::shared_ptr;
using std::make_shared;

//Project Namespaces
using namespace restbed;

//External Namespaces

Response get_handler( const Request& )
{
    Response response;
    response.set_status_code( StatusCode::OK );
    
    return response;
}

TEST( Service, mismatched_resource_path )
{
    Resource resource;
    resource.set_path( "test" );
    resource.set_method_handler( "GET", &get_handler );
    
    Settings settings;
    settings.set_port( 1984 );
    settings.set_mode( ASYNCHRONOUS );
    
    auto service = make_shared< Service >( settings );
    service->publish( resource );
    
    service->start( );
    
    auto response = Http::get( "http://localhost:1984/" );
    
    EXPECT_EQ( "404", response[ "Status Code" ] );
    
    service->stop( );
}

TEST( Service, matched_resource_path )
{
    Resource resource;
    resource.set_method_handler( "GET", &get_handler );
    
    Settings settings;
    settings.set_port( 1984 );
    settings.set_root( "test" );
    settings.set_mode( ASYNCHRONOUS );
    
    auto service = make_shared< Service >( settings );
    service->publish( resource );
    
    service->start( );
    
    auto response = Http::get( "http://localhost:1984/test" );
    
    EXPECT_EQ( "200", response[ "Status Code" ] );
    
    service->stop( );
}
