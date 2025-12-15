#!/bin/bash

# Test script for Advanced Search API

echo "Testing Advanced Search API endpoints..."

API_BASE="http://localhost:3000/api"

echo ""
echo "1. Testing basic search endpoint (all trips):"
curl -s "${API_BASE}/trips" | jq '. | length'

echo ""
echo "2. Testing price range search (trips under $2000):"
curl -s "${API_BASE}/trips/search?maxPrice=2000" | jq '. | length'

echo ""
echo "3. Testing description keyword search (beach):"
curl -s "${API_BASE}/trips/search?description=beach" | jq '. | length'

echo ""
echo "4. Testing duration search (5 days):"
curl -s "${API_BASE}/trips/search?duration=5" | jq '. | length'

echo ""
echo "5. Testing name search (adventure):"
curl -s "${API_BASE}/trips/search?name=adventure" | jq '. | length'

echo ""
echo "6. Testing combined search (price + description):"
curl -s "${API_BASE}/trips/search?maxPrice=3000&description=luxury" | jq '. | length'

echo ""
echo "Test completed!"