import pandas as pd
import re

df = pd.read_csv("prado.csv")

print(df.columns)

# keywords indicating a painting: oil, linen, tempera, acrylic, painting
painting_keywords = ["óleo", "lienzo", "temple", "acrílico", "pintura"]


# extract year from subtitle
def extract_year(text):
    if pd.isna(text):
        return None
    match = re.search(r"\b\d{4}\b", text)  # looks for 4 digit int = year
    return int(match.group()) if match else None


df["year_created"] = df["work_subtitle"].apply(extract_year)

#  extract year from description if subtitle doesn't exist)
df.loc[df["year_created"].isna(), "year_created"] = df[
    "work_description"
].apply(extract_year)


# identify paintings from their medium (in subtitles)
def is_painting(text):
    if pd.isna(text):
        return False
    text_lower = text.lower()
    return any(keyword in text_lower for keyword in painting_keywords)


df["is_painting"] = df["work_subtitle"].apply(is_painting) | df[
    "work_description"
].apply(is_painting)

# filter only paintings with int years
filtered_df = df[(df["is_painting"]) & (df["year_created"].notna())]

# save new dataset
filtered_df.to_csv("filtered_paintings_with_years.csv", index=False)
