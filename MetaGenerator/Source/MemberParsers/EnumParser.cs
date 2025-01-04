using PCRE;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;

namespace MetaGenerator
{
    internal class EnumParser : MemberParser
    {
        public EnumParser(HeaderParser headerParser) : base(headerParser)
        {
        }

        public EnumInfo[] Parse()
        {
            IEnumerable<PcreMatch> matches = FindMatches(@"REFLECTABLE(?<rec>\((?:[^()]|(?&rec))*\))(\s*enum\s+(class\s+)?(\S+)[\s\S]+?){([\s\S]*?)};", HeaderParser.Text);
            return ParseInfos(matches);
        }

        private EnumInfo[] ParseInfos(IEnumerable<PcreMatch> matches)
        {
            var infos = MemberInfo.Construct<EnumInfo>(matches.Count());

            int i = 0;
            foreach (var match in matches)
            {
                infos[i].name = match.Groups[4].Value;
                infos[i].namespaceName = HeaderParser.GetNamespaceByPosition(match.Index);
                infos[i].valueNames = ParseEnumValueNames(match.Groups[5].Value);
                infos[i].forwardDeclaration = match.Groups[2].Value.Trim();

                ++i;
            }

            return infos;
        }

        private string[] ParseEnumValueNames(string values)
        {
            var matches = Regex.Matches(values, @"(\w+)\s*(=\s*\S+)?,?");

            if (matches.Count == 0)
                return null;

            var names = new string[matches.Count];

            for (int i = 0; i < matches.Count; i++)
                names[i] = matches[i].Groups[1].Value;

            return names;
        }
    }
}
