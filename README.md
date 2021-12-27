# GiphyStickers #
Pulls stickers using Giphy API using the Asio Library

# API Key #
API Key must be generate at Giphy Developers website. <br />
API Key must be for stickers Search endpoint API.

# Dependencies #
Non-Boost Asio Library is required. <br />
C++ Nholmann JSON Library is required.

# How to use #
When running the console, enter search query as requested. <br />
Stickers with their Type, Rating, and URL should appear 10 at a time. <br />
As prompted, after stickers load, enter response to prompt. <br />
&nbsp; 'next'   : next 10 pages of stickers will be pulled.<br />
&nbsp; 'search' : conducts a brand new search, previous search history will be cleared.<br />
&nbsp; 'rank'   : searches for all stickers with the same rating within search history.<br />
&nbsp; 'end'    : terminate the program.<br />
