# GiphyStickers #
Pulls stickers using Giphy API using the Asio Library

# API Key #
API Key must be generate at Giphy Developers website
API Key must be for stickers Search endpoint API

# Dependencies #
Non-Boost Asio Library is required.
C++ Nholmann JSON Library is required.

# How to use #
When running the console, enter search query as requested.
Stickers with their Type, Rating, and URL should appear 10 at a time.
As prompted, after stickers load, enter response to prompt.
  'next'   : next 10 pages of stickers will be pulled.
  'search' : conducts a brand new search, previous search history will be cleared.
  'rank'   : searches for all stickers with the same rating within search history.
  'end'    : terminate the program.
