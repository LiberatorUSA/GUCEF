# GUCEF

This is the main mono repo for the Galaxy Unlimited Platform.
The software contained herein is a collection of libraries and derived services and applications all fairly generic in nature.

About the name:
GUCEF stood for "Galaxy Unlimited Client Engine Framework" at one time. It was part of a multi-tier stack that had independent GUCE and GU software stack tiers.
GUCE stood for "Galaxy Unlimited Client Engine".
GUC stood for "Galaxy Unlimited Client".
GU is still the abbreviation for "Galaxy Unlimited".
At this time GUCEF and GUCE have merged together and some parts of GUC and GU have been incorporated as well.
Instead of being client-side focused at lot of the new development has focused on supporting services and generally back end development.

The repo breakdown is as follows:
- /platform : this has the main platform modules
- /plugins : this has various plugins that can optionally be used to extend platform capabilities
- /projects : this has scripts and the like for generating build projects
- /common/bin : this has binary output of projects
