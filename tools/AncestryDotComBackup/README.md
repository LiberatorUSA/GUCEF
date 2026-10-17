# AncestryDotComBackup

Makes a lossless local backup of an Ancestry.com family tree. The tool drives a Chrome (or Chromium) browser which the
user logs in to by hand, so no password is ever stored, and learns from that browser how the site gets its data.

Status: `login`, `discover`, `baseline`, `export`, `changes`, `media` and `report` work. The crawler, media downloader, converters and verifier come next, once the
endpoints found by `discover` have been reviewed.

## Commands

    AncestryDotComBackup login      treeId=<id> [dataDir=<dir>]
    AncestryDotComBackup discover   treeId=<id> [dataDir=<dir>]
    AncestryDotComBackup export     treeId=<id> [dataDir=<dir>] [exportDirs=<dirs>] [exportMaxAgeDays=7]
    AncestryDotComBackup changes    treeId=<id> [dataDir=<dir>] [changesSince=<time>] [changesMaxPages=50]
    AncestryDotComBackup media      treeId=<id> [dataDir=<dir>] [maxRequestsPerRun=500]
    AncestryDotComBackup report     treeId=<id> [dataDir=<dir>]
    AncestryDotComBackup baseline   treeId=<id> gedcom=<file.ged> [dataDir=<dir>]

* `login` starts the browser with a profile of its own (`<dataDir>/<treeId>/browser-profile`), opens Ancestry, waits for
  you to log in and press Enter, then checks that the browser holds cookies of the site. The browser stays open, the
  login lives in its profile.
* `discover` connects to that browser and records every request the site makes while you click through a person: facts,
  sources, media, comments, the story, an attached record, the media gallery. Press Enter when done. Requests go to
  `<treeId>/discovery/requests.jsonl`, JSON responses are saved in `discovery/bodies/`, and `discovery/endpoints.md`
  describes the endpoints: methods, paths with ids replaced by `{n}`, query parameters, paging fields, urls in
  responses and the shape of the JSON (field names and types, not the data). Cookies and authorization headers are
  never recorded. Only hosts that contain one of the `hostFilter` terms are recorded, so other tabs are left alone.
  Responses are saved as they are, they hold your tree.
* `baseline` takes in the GEDCOM export of the tree (Tree Settings, Export tree), the sanctioned starting point of a
  backup. It is optional. The file is checked, copied to `<treeId>/baseline.ged`, and `state/baseline-summary.txt`
  says what it holds (people, families, sources, media files, source record ids) and what a GEDCOM cannot hold, such
  as comments, stories and transcriptions. `state/baseline-individuals.txt` lists the person ids. Run it again
  without `gedcom=` to check the file that was imported earlier.
* `export` looks in your downloads directory (or `exportDirs`) for a GEDCOM file written by Ancestry within the last
  `exportMaxAgeDays` days and takes in the newest one that was not taken in yet, as `baseline` does. Every export that
  is taken in is also kept as `<treeId>/baselines/export-<time>.ged`, so exports can be compared later. It ends with
  exit code 5 when there is no recent export. It does nothing on the site, and zip files are not opened.
* `changes` asks the site's change feed what changed in the tree since the last run, or since the baseline export the
  first time, and writes `state/changes.jsonl` (what changed), `state/changed-people.txt` (the people to look at again)
  and keeps the feed pages it read under `raw/changes/`. A day's changes are a request or two.
* `media` lists the media of the tree (photos, documents, stories, recordings, videos), 28 per page, into
  `state/media-inventory.jsonl`, and downloads the files of the pictures and documents in full size to `media/<id>.<ext>`
  with `media/<id>.<ext>.json` next to each, holding everything the site said about the item. It goes on where the last run
  stopped and never fetches a file that is there. Failures are written to `state/media-failed.txt`. Stories, recordings and
  videos are in the inventory but not downloaded yet.
* `report` rebuilds `endpoints.md` from `requests.jsonl`, no browser needed.

**Using the browser you already have open.** Instead of a browser of its own the tool can use your everyday Chrome, so
your own login (for example "sign in with Google") is used. Open `chrome://inspect/#remote-debugging` in that Chrome and
tick "Allow remote debugging for this browser instance". The tool finds it through the `DevToolsActivePort` file in the
browser's profile and Chrome asks you to allow the connection each time the tool connects. It opens a tab of its own
for the site, and only tabs that show a host matching `hostFilter` are watched. Switch the option off again afterwards.
This is the default when such a browser is found; `attachToRunning=false` always starts a browser of its own.

**Requests to the site.** The tool never sends anything but GET requests, and only ones that are on an allowlist of
requests known to read (`AncestryBackup_CRequestPolicy`), for the tree it was set up for and no other, never to
sign-in or account pages and never to DNA pages unless the DNA facility asks. `discover` leaves the DNA pages out of its
journal and report too, unless `recordDna=true` is given; sign-in and account pages are never recorded. More endpoints can be allowed in
`<treeId>/endpoints.ini`, one path template per line. Requests are made from inside your own browser tab, one every
`requestDelayMs` (default 2.5 s, varied by up to `requestJitterPercent`), at most `maxRequestsPerRun` per run and
`maxRequestsPerDay` per day. If the site answers 429, 403 or 401, serves a challenge, or answers a page of html where data
was asked for, the tool writes `state/HALTED.txt`, stops, and refuses to make requests in any later run until you
have looked at that file and deleted it. There are no retries.

Settings come from `AncestryDotComBackup.ini` and from the command line (`name=value` or `--name=value`), the command
line wins. See `AncestryDotComBackup --help`. Exit codes: 0 ok, 1 usage, 2 no browser, 3 not logged in, 4 failed.

## Layout

* `tools/AncestryDotComBackupLib`: the logic, as a static library.
* `tools/AncestryDotComBackup`: the command line tool.
* `tests/AncestryDotComBackup_TestApp`: tests on the GUCEF test framework. The end to end test starts a headless Chrome
  against a small local site; it is skipped when there is no Chrome or Chromium (set `GUCEF_TEST_CHROME_NO_SANDBOX=1`
  to run Chrome without its sandbox, for containers).

## Notes

Automated access is against the terms of use of Ancestry. The tool only reads your own tree, from your own session.
Start with a small test, and keep the rate low.
