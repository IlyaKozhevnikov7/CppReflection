﻿using PCRE;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;

namespace MetaGenerator
{
    internal class FieldParser : MemberParser
    {
        public FieldParser(string text) : base(text)
        {
        }

        public FieldInfo[] Parse()
        {
            IEnumerable<PcreMatch> matches = FindMatches(@"FIELD(?<rec>\((?:[^()]|(?&rec))*\))([\s\S]*?;)", Text);

            if (matches.Count() == 0)
                return null;

            return ParseInfos(matches);
        }

        private FieldInfo[] ParseInfos(IEnumerable<PcreMatch> matches)
        {
            var infos = MemberInfo.Construct<FieldInfo>(matches.Count());

            int i = 0;
            foreach (var match in matches)
            {
                var fieldDeclaration = match.Groups[2].Value;

                var nameMatch = Regex.Match(fieldDeclaration, @"(\S+)\s*(=[\s\S]+)?;");
                infos[i].name = nameMatch.Groups[1].Value;
                infos[i].attributeInfos = ParseAttributes(match.Groups["rec"].Value);

                ++i;
            }

            return infos;
        }
    }
}
